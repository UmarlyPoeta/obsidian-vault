---
typ: solar-slam
tags: [solar, slam, l2]
zaktualizowano: 2026-10-08
---
# SLAM — lokalna mapa voxelowa

> [!skrot] W 30 sekund
> - `LocalVoxelMap` (`pose_graph/include/local_voxel_map.hpp`, header-only) = `unordered_map<VoxelKey, MapVoxel>` z voxelami **0,40 m** w układzie `map`. Każdy voxel trzyma $n$, $\sum p$, $\sum p p^T$, liczbę punktów z zaufanych keyframe’ów, a po `finalize()` — centroid, kowariancję, normalną/kierunek i typ PLANE/LINE.
> - Mapa **nie jest aktualizowana przyrostowo**: przy każdym nowym keyframe jest czyszczona i budowana od zera z punktów wszystkich (≤ 10) keyframe’ów (`FeatureExtractor::rebuildLocalMap`, `feature_extractor.cpp:2119-2132`).
> - „Usuwanie” = tylko wypadnięcie najstarszego keyframe’u z kolejki (`pop_front`, `:2107-2108`) i usunięcie voxeli bez typu w `finalize()`. Brak starzenia, brak usuwania wg odległości.
> - Wyszukiwanie korespondencji: przegląd sześcianu $(2r+1)^3$ voxeli wokół zapytania, $r = \lfloor d_c / s \rfloor + 1 = 3$ → **343 odwołania do hash-mapy na punkt**; wybór najmniejszego `score = d + 0,05·d_c + 0,05·(1 − quality)`.
> - Przy keyframe co 0,8 m okno 10 keyframe’ów to tylko ~8 m przebytej drogi — mapa jest bardzo „krótka”, dryf narasta szybko, co jest główną motywacją dla loop closure.

Kontekst: [[SLAM — architektura węzła i przepływ danych]], [[SLAM — cechy i klasyfikacja]], [[SLAM — predykcja pozy, keyframe’y i odporność]].

## Struktury

| struktura | linie | zawartość |
|---|---|---|
| `LocalVoxelMapConfig` | `local_voxel_map.hpp:23-32` | `voxel_size` 0,40, `minimum_points_per_voxel` 5, `minimum_linearity` 0,50 (w węźle nadpisane domyślnym 0,45), `minimum_planarity` 0,35 (w węźle 0,32), `maximum_scattering` 0,12, `plane_dominance_margin` 0,02 |
| `VoxelKey` | `:34-46` | `(ix, iy, iz)` int |
| `VoxelKeyHash` | `:48-59` | mieszanie w stylu `boost::hash_combine` (stała `0x9e3779b9`) |
| `MapVoxel` | `:61-166` | `count`, `trusted_count`, `sum`, `sum_outer`, `centroid`, `covariance`, `eigenvalues`, `normal`, `direction`, `linearity`, `planarity`, `scattering`, `quality`, `trusted_fraction`, `type` |
| `PlaneCorrespondence` / `LineCorrespondence` | `:168-184` | centroid, normalna lub kierunek, odległość, `quality`, `trusted_fraction` — wynik wyszukiwania dla matchera |

Konfiguracja jest składana w konstruktorze węzła z `map.*` (`feature_extractor.cpp:242-273`, mapa tworzona `:350-351`).

## Wstawianie

`insertCloud(cloud, T_map_lidar, trusted)` (`:234-257`): dla każdego skończonego punktu $p^{map} = T_{map\_lidar}\,p^{lidar}$ i `addPoint` (`:225-232`) → klucz $\lfloor p^{map}/s \rfloor$ → `MapVoxel::addPoint` (`:86-96`):

$$n \mathrel{+}= 1,\quad \Sigma_1 \mathrel{+}= p,\quad \Sigma_2 \mathrel{+}= p\,p^T,\quad n_{trusted} \mathrel{+}= [\text{trusted}]$$

Źródłem punktów są chmury keyframe’ów: `filtered_` (voxel 0,10 m) ponownie przerzedzone do **0,18 m** (`map.keyframe_voxel_size`, `addKeyframe` `:2093-2094`), w układzie lidaru na początek skanu. Do mapy idą **wszystkie** punkty keyframe’u, nie tylko cechy — typ voxela wynika z rozkładu punktów w voxelu, a nie z klasyfikacji per-punkt.

## Finalizacja voxela (`MapVoxel::finalize`, `:98-165`)

- $n < 5$ → UNKNOWN (i usunięcie).
- $c = \Sigma_1 / n$, `trusted_fraction` $= n_{trusted}/n$,

$$C = \frac{\Sigma_2 - n\,c\,c^T}{n - 1}$$

(estymator nieobciążony; symetryzacja `:118-119`). Rozkład `SelfAdjointEigenSolver<Matrix3d>`, $\lambda$ obcięte do ≥ 0, $\lambda_2 < 10^{-10}$ → UNKNOWN.
- `normal` = wektor własny $\lambda_0$, `direction` = wektor własny $\lambda_2$ (`:136-137`).
- $L, P, S$ jak w klasyfikatorze, ta sama reguła dominacji (`:139-164`); progi z `map.*`: $L_{min}$ 0,45, $P_{min}$ 0,32, $S_{max}$ 0,12, margines 0,02. `quality` $= P(1-S)$ dla PLANE, $L(1-S)$ dla LINE.

`LocalVoxelMap::finalize` (`:259-283`) wywołuje to dla wszystkich voxeli, usuwa UNKNOWN (`erase`), liczy `plane_count_`, `line_count_`. W mapie zostają więc tylko voxele-prymitywy.

> [!intuicja] Dlaczego 0,40 m i 0,18 m
> Voxel 0,40 m z punktami co ~0,18 m daje do ~10 punktów na powierzchnię voxela z jednego keyframe’u, a z kilku keyframe’ów więcej — wystarczy na stabilną kowariancję przy progu 5 punktów. Większy voxel lepiej uśrednia, ale miesza różne powierzchnie (narożnik = „linia” albo nic).

## Wyszukiwanie korespondencji

`findNearestPlane(query, result, d_max = 0,25, d_c = 0,80)` (`:310-376`) i `findNearestLine(query, result, d_max = 0,35, d_c = 0,80)` (`:378-448`) — progi przekazuje matcher z `matcher.maximum_plane_distance`, `maximum_line_distance`, `maximum_center_distance` (wywołania `scan_matcher.cpp:342-343`, `:430-431`). `query` = punkt źródłowy już przekształcony bieżącą estymatą do `map`.

- Promień w voxelach: `neighborRadius` (`:458-467`) $r = \max(1, \lfloor d_c/s \rfloor + 1)$; dla 0,80/0,40 → $\lfloor 2{,}0\rfloor + 1 = 3$.
- Dla każdego voxela w sześcianie $(2r+1)^3$: tylko właściwy typ; odrzuć, gdy $\lVert q - c\rVert > d_c$.
  - płaszczyzna: $d = |n^T (q - c)|$, odrzuć gdy $d > d_{max}$;
  - linia: $d = \lVert (q - c) - u\,u^T(q - c) \rVert$, odrzuć gdy $d > d_{max}$;
  - $\text{score} = d + 0{,}05\,\lVert q - c\rVert + 0{,}05\,(1 - \mathrm{clamp}(\text{quality}, 0, 1))$ — minimalny wygrywa (`:355-370`, `:427-442`).
- Wynik niesie `trusted_fraction` voxela — matcher sumuje je do `trusted_match_fraction` (`scan_matcher.cpp:534-538`), co blokuje akceptację dopasowań opartych głównie na recovery-keyframe’ach.

Koszt: zapytań na iterację ≈ (3500 + 2000) × 343 ≈ 1,9 mln `find` w hash-mapie, × do 8 iteracji (`matcher.maximum_iterations`) → do ~15 mln odwołań na skan (górne oszacowanie; zależne od liczby cech po próbkowaniu).

## Odbudowa i publikacja

- `rebuildLocalMap` (`feature_extractor.cpp:2119-2132`): `clear()` → `insertCloud` każdego keyframe’u z jego `pose` i `trusted` → `finalize()`. Koszt O(suma punktów keyframe’ów), wykonywany w `hesaiCallback` przy każdym nowym keyframe.
- `publishLocalMap` (`:2134-2208`): `/local_voxel_map` (centroidy, intensity 1 = PLANE, 2 = LINE) i `/slam/local_map_points` (wszystkie punkty keyframe’ów w `map`, intensity 1,0 trusted / 0,25 recovery), oba transient_local, stempel `now()` (nie czas skanu).

## Podejrzane / do naprawy

1. **Promień wyszukiwania o 1 za duży**: centroid voxela leży w voxelu, więc dla $\lVert q - c \rVert \le d_c$ wystarcza $r = \lceil d_c / s \rceil = 2$ (125 voxeli zamiast 343, ~2,7× mniej odwołań). Obecne $+1$ jest bezpieczne, ale kosztowne. Propozycja: `std::ceil(d_c * inverse_voxel_size_)`.
2. **Pełna odbudowa przy każdym keyframe**: przy 10 keyframe’ach tanio, ale przy próbie powiększenia okna (np. do 100 dla lepszej lokalnej spójności) koszt rośnie liniowo. Alternatywa: przyrostowe `addPoint` + odejmowanie statystyk usuwanego keyframe’u (statystyki $\Sigma_1, \Sigma_2$ są addytywne, więc da się je odjąć).
3. **Okno po liczbie, nie po przestrzeni**: 10 keyframe’ów × 0,8 m ≈ 8 m (albo 10 × 4° obrotu w miejscu). Na postoju (bez ruchu ≥ 0,25 m) keyframe’ów nie przybywa, więc mapa się nie odświeża — dobrze dla statycznej sceny, źle dla ruchomych obiektów (inne łodzie zostają w mapie na zawsze).
4. Wariancja przez $\Sigma_2 - n c c^T$ traci precyzję przy dużych współrzędnych; w double i skali portu (≤ kilka km) bez znaczenia — nie przenosić do float.
5. `minimum_linearity` / `minimum_planarity` mapy mają **inne domyślne w kodzie węzła** (0,45 / 0,32, `feature_extractor.cpp:255-263`) niż w `LocalVoxelMapConfig` (0,50 / 0,35) — mylące przy czytaniu nagłówka.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
