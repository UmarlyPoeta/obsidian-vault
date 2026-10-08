---
typ: solar-slam
tags: [solar, slam, l2]
zaktualizowano: 2026-10-08
---
# SLAM — cechy i klasyfikacja

> [!skrot] W 30 sekund
> - Po deskew chmura jest czyszczona (NaN, zasięg < 0,1 m), uśredniana w voxelach 0,10 m, a dla **każdego** punktu liczona jest PCA z k = 20 najbliższych sąsiadów (`Classifier::classify`, `pose_graph/src/classifier.cpp:20-152`).
> - Z wartości własnych kowariancji $\lambda_0 \le \lambda_1 \le \lambda_2$: liniowość $L = (\lambda_2-\lambda_1)/\lambda_2$, płaskość $P = (\lambda_1-\lambda_0)/\lambda_2$, rozproszenie $S = \lambda_0/\lambda_2$ (zawsze $L + P + S = 1$).
> - Płaszczyzna: $P \ge 0{,}35 \wedge S \le 0{,}12$; linia (EDGE): $L \ge 0{,}45 \wedge S \le 0{,}12$; gdy oba — płaszczyzna, jeśli $P + 0{,}02 \ge L$. Reszta → `UNKNOWN` („debris”).
> - Do matchera idą tylko **pozycje punktów** (nie normalne ani kierunki): najlepsze wg wyniku $P(1-S)$ / $L(1-S)$, po jednym na komórkę 0,25 m (płaszczyzny, max 3500) i 0,20 m (linie, max 2000).
> - Te same definicje i reguła dominacji są w mapie voxelowej ([[SLAM — lokalna mapa voxelowa]]), tylko z innymi progami.

Kontekst: [[SLAM — architektura węzła i przepływ danych]], poprzedni krok: [[SLAM — deskewing]].

## Przygotowanie chmury (`hesaiCallback`)

1. `removeInvalidPoints` (`feature_extractor.cpp:1642-1668`): usuwa NaN/Inf i punkty z $x^2+y^2+z^2 < 0{,}01$ (zasięg < 0,1 m). Brak filtra maksymalnego zasięgu, brak filtra „własnej łodzi” (niepewne: czy driver Hesai robi to sam).
2. `voxelDownsample(input_, scan_voxel_size_ = 0,10)` (`:1670-1740`): klucz $\lfloor p / 0{,}10 \rfloor$ (int), punkt wyjściowy = **centroid** voxela, intensywność = średnia, `ring` i `timestamp` z pierwszego punktu. `unordered_map` → kolejność punktów wyjściowych losowa (bez znaczenia dalej).
3. Za mało punktów: `filtered_->size() < max(features.neighbors, 3)` → skan pominięty (`:2381-2392`).
4. `tree_->setInputCloud(filtered_)` (KdTree FLANN, `:2394`).
5. `NormalEstimationOMP` z $k = \min(20, N)$ (`:2396-2403`) — normalne zapisywane w `Feature::normal`, **nigdzie dalej nieużywane** (patrz „Podejrzane”).

## PCA sąsiedztwa (`classifier.cpp`)

Dla każdego punktu $i$ (`:49-151`):
- `nearestKSearch(p_i, k)`, $k = \max(3, \min(\texttt{neighbors}, N))$ (`:32-36`). Jeśli znaleziono < k albo odległość $k$-tego sąsiada > `maximum_neighbor_radius` (0,75 m; PCL zwraca kwadraty odległości, porównanie z $r^2$ `:38-41`, `:61-65`) → **punkt pomijany całkowicie** (nie trafia nawet do debris).
- Centroid `pcl::compute3DCentroid`, kowariancja `pcl::computeCovarianceMatrix` (`:67-79`) — ta wersja PCL zwraca **nienormalizowaną** sumę $\sum_j (p_j - c)(p_j - c)^T$; współczynniki $L, P, S$ są ilorazami, więc skala nie ma znaczenia. Symetryzacja $C \leftarrow \tfrac12 (C + C^T)$ (`:81-82`).
- `SelfAdjointEigenSolver<Matrix3f>` (float), wartości rosnąco, obcięte od dołu do 0 (`:84-91`); $\lambda_2 < 10^{-8}$ → pomiń (`:97-98`).

$$L = \frac{\lambda_2 - \lambda_1}{\lambda_2},\qquad P = \frac{\lambda_1 - \lambda_0}{\lambda_2},\qquad S = \frac{\lambda_0}{\lambda_2}$$

(`:100-107`). To są wartości własne **wariancji**, nie odchylenia standardowe (część literatury, np. Demantké, używa $\sigma_k = \sqrt{\lambda_k}$) — progi trzeba stroić pod tę definicję.

Decyzja (`:109-148`):

$$\text{line\_cand} = L \ge L_{min} \wedge S \le S_{max},\qquad \text{plane\_cand} = P \ge P_{min} \wedge S \le S_{max}$$

- `PLANE`, gdy `plane_cand` i (`!line_cand` lub $P + m \ge L$); `score` $= P\,(1 - S)$;
- inaczej `EDGE`, gdy `line_cand`; `score` $= L\,(1 - S)$;
- inaczej `UNKNOWN`, `score = 0`.

Wartości (yaml `features.*`): $L_{min} = 0{,}45$, $P_{min} = 0{,}35$, $S_{max} = 0{,}12$, $m$ = `plane_dominance_margin` = 0,02. Zapisywane też `eigenvalues` i `direction` = wektor własny $\lambda_2$ (`:123-129`) — także nieużywane przez matcher.

> [!przyklad] Kiedy punkt jest jednocześnie kandydatem na linię i płaszczyznę
> $\lambda = (0{,}05;\ 0{,}50;\ 1{,}0)$ → $L = 0{,}50$, $P = 0{,}45$, $S = 0{,}05$. Oba testy przechodzą; $P + 0{,}02 = 0{,}47 < 0{,}50 = L$ → EDGE. Przy $\lambda_1 = 0{,}52$: $L = 0{,}48$, $P = 0{,}47$, $0{,}49 \ge 0{,}48$ → PLANE.

Publikacja `/features/edges`, `/features/planes`, `/features/debris` (`feature_extractor.cpp:1845-1877`): punkty `filtered_` w układzie lidaru (początek skanu), nagłówek `lidar_header` (frame z drivera, stempel $t_0$). Te topiki czyta też `GlobalMapBuilder` (warstwa dynamiczna costmapy).

## Próbkowanie przestrzenne (`spatialSample`)

`feature_extractor.cpp:1742-1794`, wywołania `:2408-2439`:
1. podział cech na `plane_features` i `line_features` (`ScoredPoint{point, score}`), UNKNOWN odrzucone;
2. sortowanie malejąco po `score`;
3. przechodzenie po kolei, wstawianie klucza $\lfloor p / \texttt{leaf} \rfloor$ do `unordered_set`; zajęta komórka → pomiń; stop po `maximum_points`.

Wynik: najlepszy punkt w każdej komórce 0,25 m (płaszczyzny, ≤ 3500) i 0,20 m (linie, ≤ 2000), jako `std::vector<Eigen::Vector3d>` w układzie lidaru — wejście `ScanMatcher::align`. Siatka jest w układzie lidaru, więc nie zależy od pozy.

## Złożoność i czas

- Voxel: O(N). KdTree: O(N log N). Normalne OMP: N zapytań kNN (wielowątkowo). Klasyfikator: drugie N zapytań kNN + N rozkładów 3×3 (jednowątkowo). Sortowanie: O(F log F).
- Najdroższe są dwa przebiegi kNN; jeden (normalne) jest zbędny.

## Podejrzane / do naprawy

1. **Zbędne `NormalEstimationOMP`** (`feature_extractor.cpp:2396-2403`): `Feature::normal` nie jest czytane nigdzie (`grep` w `src/`), a klasyfikator i tak liczy własną PCA. Usunięcie = mniej więcej o połowę mniej czasu kNN. Uwaga: wtedy usunąć też argument `normals` z `classify`.
2. **Dwa razy to samo kNN**: można liczyć PCA w jednym przebiegu (np. `NormalEstimationOMP` z własnym callbackiem albo OpenMP w `classify`).
3. **Punkty odrzucone przez promień nie trafiają do debris** — `/features/debris` nie pokazuje całej reszty chmury; przy diagnostyce w RViz łatwo to źle zinterpretować.
4. `features.maximum_neighbor_radius` 0,75 m przy voxelu 0,10 m: daleko od lidaru (rzadkie pierścienie) większość punktów nie ma 20 sąsiadów w 0,75 m → cechy tylko blisko łodzi. To może być zamierzone, ale ogranicza zasięg dopasowania (i potencjalne deskryptory miejsc).
5. Klasyfikacja w float (`Matrix3f`) przy współrzędnych lokalnych lidaru — OK; nie przenosić bez zmiany na współrzędne mapy (utrata precyzji).

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
