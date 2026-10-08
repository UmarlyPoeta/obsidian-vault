---
typ: solar-slam
tags: [solar, slam, l2]
zaktualizowano: 2026-10-08
---
# SLAM — degeneracja i jakość dopasowania

> [!skrot] W 30 sekund
> Matcher (`pose_graph/src/scan_matcher.cpp:140–289`) przed każdym krokiem liczy z **samego Hessianu danych** $H$ (bez priora) informację translacji po wyeliminowaniu yaw (dopełnienie Schura), normalizuje ją liczbą wymiarów residuów i rozkłada na wartości własne. Kierunek translacji jest „obserwowalny”, gdy wartość własna $\ge 0.01$ **i** stosunek do największej $\ge 0.02$. Yaw jest obserwowalny, gdy jego dopełnienie Schura znormalizowane przez $N\cdot\overline{r^2}$ $\ge 0.005$. Krok jest liczony **tylko** w obserwowalnych kierunkach; w słabych zostaje predykcja z odometrii. Rząd translacji < 1 → `DEGENERATE_GEOMETRY`. Kowariancji wynik **nie zwraca** — do grafu póz trzeba ją dopisać (przepis niżej).

Stan: drzewo robocze `~/solar/l2dp_docking_fix` (bez zmian w `scan_matcher.*` względem HEAD `0d444ff`). Matematyka residuów: [[SLAM — scan matching (matematyka)]]. Parametry: [[SLAM — parametry back-endu]]. Test: [[SLAM — plan testów i weryfikacja]].

## Dlaczego to jest potrzebne (intuicja)

> [!intuicja] Korytarz / jedna ściana
> Jeśli LiDAR widzi tylko jedną długą ścianę równoległą do osi Y mapy, residua punkt–płaszczyzna mają normalną $n = (1,0,0)$, więc $\partial r/\partial\delta y = n_y = 0$. Przesunięcie w Y nie zmienia kosztu — $H$ jest osobliwy w tym kierunku. Bez ochrony solver (z małym tłumieniem) i tak „coś” policzy w Y z szumu, a powtarzane co skan korekty mogą **zjadać** poprawny ruch z odometrii. To przyczyna nr 1 z audytu autora (`REVIEW_AND_TEST_PLAN.md:29`).

## Informacja translacji (dopełnienie Schura)

Oznaczenia z kodu: `H` (3×3, symetryzowany, `scan_matcher.cpp:149–150`), `H_translation` = $H_{tt}$ (2×2), `H_translation_yaw` = $h_{t\psi}$ (2×1), `H_yaw` = $H_{\psi\psi}$. $N_{dim}$ = `residual_dimensions`.

> [!wzor] Informacja translacji po marginalizacji yaw (`scan_matcher.cpp:152–176`)
> $$S_{xy} = \frac{1}{N_{dim}}\left(H_{tt} - \frac{h_{t\psi} h_{t\psi}^T}{H_{\psi\psi}}\right)\quad (\text{gdy } H_{\psi\psi} > 10^{-10};\ \text{inaczej bez odejmowania})$$
> Wartości własne $\lambda_0 \le \lambda_1$ (`SelfAdjointEigenSolver`, rosnąco), obcięte do $\ge 0$ (`:183–184`); wektory własne $v_0, v_1$ (`:186–187`). Jednostka: [–] (bo $\partial r/\partial t$ to składowe normalnej), ale **skalowane wagami** $w$.

> [!wzor] Test kierunku translacji (`scan_matcher.cpp:192–219`)
> $$\text{obserwowalny}_i \iff \lambda_i \ge \texttt{minimum\_translation\_information}\ (0.01)\ \wedge\ \lambda_{max} > 10^{-12}\ \wedge\ \frac{\lambda_i}{\lambda_{max}} \ge \texttt{minimum\_translation\_eigenvalue\_ratio}\ (0.02)$$
> Każdy obserwowalny $v_i$ trafia jako kolumna $[v_i; 0]$ do bazy $B$; `translation_rank` = liczba takich kolumn.

Raportowane: `translation_information_eigenvalues` = $(\lambda_0, \lambda_1)$, `weak_translation_direction` = $v_0$ (zawsze kolumna 0, nawet gdy oba obserwowalne) (`:567–571`).

## Informacja yaw

> [!wzor] Yaw (`scan_matcher.cpp:222–245`)
> $$s_\psi = \max\!\left(0,\ H_{\psi\psi} - h_{t\psi}^T\, H_{tt}^{+}\, h_{t\psi}\right),\qquad \texttt{yaw\_information} = \frac{s_\psi}{N_{dim}\cdot \max(\overline{\|(Rp)_{xy}\|^2}, 1)}$$
> $H_{tt}^{+}$ — pseudoodwrotność 2×2 (`pseudoInverseSymmetric2x2`, `:108–138`: wartości własne $\le \max(10^{-10}, 10^{-6}\lambda_{max})$ zerowane). $\overline{\|(Rp)_{xy}\|^2}$ — średni kwadrat poziomego promienia punktów dopasowanych (`meanHorizontalRadiusSquared`, `:40–49`, min 1 m²). Obserwowalny, gdy $\ge$ `minimum_yaw_information` (0.005) → do $B$ dochodzi kolumna $e_\psi$.

Normalizacja przez $\overline{r^2}$ sprawia, że `yaw_information` jest bezwymiarowe (odpowiada informacji o „łuku” $r\,\delta\psi$ w metrach).

## Liczba uwarunkowania (tylko diagnostyka)

> [!wzor] (`scan_matcher.cpp:247–286`)
> $$u = [\delta x, \delta y, \bar r\,\delta\psi],\quad D = \mathrm{diag}(1,1,1/\bar r),\quad \hat H = \frac{D^T H D}{N_{dim}},\quad \kappa = \frac{\lambda_{max}(\hat H)}{\min\{\lambda_i(\hat H) > 10^{-10}\}}$$
> `normalized_condition_number` nie wchodzi do żadnej bramki — jest tylko w wyniku (i **nie** jest logowany w linii `SLAM status=`, `feature_extractor.cpp:2657–2690`).

## Jak degeneracja wpływa na krok

- Bramka: `translation_rank < minimum_observable_translation_rank` (clamp 1..2, `feature_extractor.cpp:513–519`) lub `basis_columns == 0` → natychmiast `DEGENERATE_GEOMETRY`, poza = ostatnia z iteracji (`scan_matcher.cpp:687–699`). Druga część warunku jest martwa (rząd ≥ 1 ⇒ ≥ 1 kolumna).
- Przy rzędzie 1 krok jest liczony w bazie $B = [v_1, (e_\psi)]$: składowa wzdłuż $v_0$ jest dokładnie 0 (`:740–782`). Sprawdziłem numerycznie (skrypt `scratchpad/slam/backend/extra_checks.py`): jedna ściana $x = 10$, 500 punktów, gradient z udziałem $y$ → przyrost $(\delta x, \delta y, \delta\psi) = (-0.0483,\ 0.0000,\ -0.0008)$.
- Prior odometrii też jest rzutowany na $B$, więc w kierunku $v_0$ nie działa ani dane, ani prior — poza zostaje z predykcji.
- Końcowa bramka rzędu (`:888–893`) używa obserwowalności w **końcowej** pozie.

> [!przyklad] Progi są w jednostkach ważonych
> Ponieważ $H$ zawiera wagi $w$, wartości własne skalują się liniowo z `plane_weight`/`line_weight`, jakością i zaufaniem. Z `extra_checks.py` (jedna ściana, 500 punktów): $\lambda_1 = 0.989$ przy $w = 1$, $0.494$ przy $w = 0.5$, $0.0297$ przy $w = 0.03$ (wciąż ≥ 0.01), $0.0148$ przy $w = 0.015$; yaw wypada z bazy przy $w = 0.015$ (`yaw_info` 0.0037 < 0.005). Zmiana `matcher.plane_weight` z 1.0 (domyślne) na 0.5 (slam.yaml) **po cichu** przesuwa efektywne progi obserwowalności o czynnik 2.

## Jakość dopasowania dostępna dziś

| Wielkość | Pole `AlignmentResult` | Znaczenie | Logowana? |
|---|---|---|---|
| RMSE nieważone | `rmse`, `initial_rmse` | $\sqrt{\sum r^2/N_{dim}}$ [m] | `rmse` tak |
| Liczby dopasowań | `plane_matches`, `line_matches`, `residual_dimensions` | | P/L tak |
| Stosunki | `match_ratio`, `plane_match_ratio`, `line_match_ratio` | | `ratio` tak |
| Zaufanie | `trusted_match_fraction` | średnie $f_{trusted}$ | tak |
| Obserwowalność | `translation_information_eigenvalues`, `weak_translation_direction`, `observable_translation_rank`, `yaw_observable`, `yaw_information` | | tak (bez `yaw_observable`) |
| Uwarunkowanie | `normalized_condition_number` | | nie |
| Korekta | `correction_x/y/translation/yaw_deg` | względem predykcji | tak |
| Zbieżność | `converged`, `iterations`, `translation_update`, `rotation_update_deg` | | nie |

Log: `feature_extractor.cpp:2657–2690` (co 2 s, throttle).

## Kowariancja do grafu póz (czego brakuje i jak to dodać)

Dziś nie ma ani $H$, ani $\Sigma$ w wyniku. Propozycja (do wdrożenia — nie ma w kodzie):

> [!wzor] Informacja krawędzi skan–mapa (propozycja)
> $$\hat\sigma^2 = \frac{\sum_i w_i\, \|r_i\|^2}{N_{dim} - 3},\qquad \Omega_{scan} = \frac{1}{\hat\sigma^2}\, H_{data}\quad (3\times 3,\ \text{w ramce map, kolejność } x, y, \psi)$$
> $H_{data}$ = `final_linearization.H` (bez priora i bez $\lambda I$). Dla kierunków nieobserwowalnych: $\Omega \leftarrow B B^T \Omega B B^T$ (zero informacji w $v_0$), żeby graf nie „wierzył” w słaby kierunek.

Uwagi:
- Wagi Hubera i jakości są heurystyczne, więc $\Omega$ trzeba **skalibrować** (np. czynnik $\alpha$ dobrany tak, by $\chi^2$ krawędzi odometrycznych w optymalizacji ≈ liczba stopni swobody). Typowo informacja z setek punktów jest za duża (zbyt pewna) — przyda się dolny limit $\Sigma$ (np. 2–5 cm, 0.2°).
- Ramka: $H$ jest liczony dla przyrostu w ramce `map` z obrotem wokół LiDAR-u (lewy przyrost yaw, translacja bez obrotu — [[SLAM — scan matching (matematyka)]]). W grafie zwykle używa się błędu w ramce lokalnej: $\Omega_{local} = A^T \Omega A$, $A = \mathrm{diag}(R_z(\psi), 1)$ dla części translacyjnej.
- Do `AlignmentResult` (`scan_matcher.hpp:31–76`) trzeba dopisać np. `Eigen::Matrix3d information`, `double residual_variance`, `Eigen::Matrix<double,3,Eigen::Dynamic> observable_basis`, a w `populateMetrics` (`scan_matcher.cpp:521–580`) je wypełnić.

## Co zmienić w matcherze pod loop closure

1. **Eksport informacji** (wyżej) — bez tego graf nie ma wag krawędzi.
2. **Osobna konfiguracja do rejestracji pętli**: obecne bramki (korekta ≤ 0.35 m / 3°, krok ≤ 0.20 m / 1.5°, `maximum_center_distance` 0.80 m, `maximum_plane_distance` 0.25 m, prior $\sigma$ 0.3 m / 2°) zakładają małe błędy predykcji. Po pętli dryf to metry. Potrzebne: wstępne zgrubne dopasowanie (deskryptor miejsca typu Scan Context + np. skan 2D / branch-and-bound / FPFH+RANSAC), potem `ScanMatcher` z drugą instancją `ScanMatcherConfig` (większe bramki, prior wyłączony lub szeroki, `minimum_trusted_match_fraction` = 0, więcej iteracji, wymóg `converged`). `ScanMatcher` to klasa z własnym `config_` (`scan_matcher.hpp:130–146`) — dwie instancje to zero zmian w kodzie klasy.
3. **Cel dopasowania = submapa**: `align` przyjmuje dowolny `LocalVoxelMap` — można zbudować `LocalVoxelMap` z zapamiętanych keyframe’ów starej okolicy i dopasować do niego bieżący skan. Wymaga przechowywania chmur keyframe’ów w ramce LiDAR-u (dziś `keyframes_` to okno max 10 — `feature_extractor.cpp:2108`, `keyframes.maximum_count`).
4. **Wynik względny**: graf potrzebuje $T_{kf_i, kf_j}$, nie tylko absolutnego `pose`. Wystarczy policzyć z `pose` i pozy submapy, ale trzeba też przenieść kowariancję (adjoint SE(2)).
5. **Weryfikacja pętli**: oprócz RMSE i rzędu — test spójności (np. ≥ 2 kandydatów zgodnych w czasie, $\chi^2$ po optymalizacji, odrzucenie w geometrii rzędu 1).
6. **3-DoF wystarcza** dla łodzi (z, roll, pitch z IMU) — graf może być SE(2) z $z$/roll/pitch przenoszonymi z odometrii.

## Podejrzane / do naprawy

> [!blad] 1. Absolutne progi informacji zależne od wag
> `minimum_translation_information` (0.01) i `minimum_yaw_information` (0.005) porównywane z $H$ zawierającym wagi typów, Hubera, jakości i zaufania (`scan_matcher.cpp:197–199`, `:237–239`). Zmiana wagi typu zmienia efektywny próg. Propozycja: normalizować przez $\sum w$ zamiast $N_{dim}$ (wtedy informacja to „średni kwadrat składowej normalnej”, niezależny od skali wag).

> [!blad] 2. Schur translacji zakłada, że yaw jest estymowany
> $S_{xy}$ zawsze odejmuje sprzężenie z yaw (`:162–168`), także gdy yaw okaże się nieobserwowalny i nie wejdzie do bazy (wtedy yaw jest zamrożony i właściwą informacją translacji byłoby $H_{tt}$). Efekt konserwatywny (zaniżenie informacji), nie groźny.

> [!blad] 3. Obserwowalność liczona od nowa w każdej iteracji
> Patrz [[SLAM — scan matching (matematyka)]] → „Podejrzane” pkt 4.

> [!blad] 4. `weak_translation_direction` raportowany zawsze
> Gdy rząd = 2, `weak` to po prostu kierunek mniejszej wartości własnej (`:570–571`) — w logu wygląda jak „słaby”, choć jest obserwowalny. Czytać razem z `obs_rank`.

## Moje notatki

> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
