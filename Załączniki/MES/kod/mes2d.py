"""Referencyjny solver MES 2D (nieustalony przepływ ciepła, element 4-węzłowy, konwekcja).

Konwencje jak w materiałach dr. Kustry:
  - jakobian J = [[dx/dξ, dy/dξ], [dx/dη, dy/dη]] w każdym punkcie całkowania,
  - [H]  = ∫ k (dN/dx dN/dxᵀ + dN/dy dN/dyᵀ) dV,
  - [Hbc] = ∫ α N Nᵀ dS po krawędziach, których OBA węzły mają flagę BC,
  - {P}  = ∫ α N t_ot dS,
  - [C]  = ∫ c ρ N Nᵀ dV,
  - schemat niejawny: ([H] + [C]/Δτ) {t1} = [C]/Δτ {t0} + {P}.

To NIE jest kod do oddania. Służy do sprawdzania własnych wyników liczbowych.
Uruchomienie:  python3 mes2d.py "../Kustra — siatki testowe/Test1_4_4.txt" [npc]
"""
import sys
import numpy as np


def wczytaj(sciezka):
    gd, wezly, elementy, bc = {}, [], [], set()
    sekcja = None
    for linia in open(sciezka, encoding="utf-8", errors="ignore"):
        s = linia.strip()
        if not s:
            continue
        if s.startswith("*"):
            sekcja = s.split(",")[0].lower()          # *node, *element, *bc
            continue
        if sekcja is None:                            # nagłówek: "Klucz wartość"
            *klucz, wartosc = s.split()
            gd["_".join(klucz).lower()] = float(wartosc)
        elif sekcja == "*node":
            _, x, y = (float(v) for v in s.split(","))
            wezly.append((x, y))
        elif sekcja == "*element":
            elementy.append([int(v) - 1 for v in s.split(",")[1:]])
        elif sekcja == "*bc":
            bc |= {int(v) - 1 for v in s.split(",") if v.strip()}
    return gd, np.array(wezly), np.array(elementy), bc


def gauss(n):
    p, w = np.polynomial.legendre.leggauss(n)
    return p, w


def N4(ksi, eta):
    return 0.25 * np.array([(1 - ksi) * (1 - eta), (1 + ksi) * (1 - eta),
                            (1 + ksi) * (1 + eta), (1 - ksi) * (1 + eta)])


def dN4(ksi, eta):
    dksi = 0.25 * np.array([-(1 - eta), (1 - eta), (1 + eta), -(1 + eta)])
    deta = 0.25 * np.array([-(1 - ksi), -(1 + ksi), (1 + ksi), (1 - ksi)])
    return dksi, deta


def macierze_elementu(xy, k, rho_c, npc):
    """Zwraca H i C jednego elementu (4x4). xy: współrzędne 4 węzłów (4x2)."""
    p, w = gauss(npc)
    H, C = np.zeros((4, 4)), np.zeros((4, 4))
    for j, eta in enumerate(p):           # kolejność punktów: najpierw ξ rośnie, potem η
        for i, ksi in enumerate(p):
            dksi, deta = dN4(ksi, eta)
            J = np.array([[dksi @ xy[:, 0], dksi @ xy[:, 1]],
                          [deta @ xy[:, 0], deta @ xy[:, 1]]])
            detJ = np.linalg.det(J)
            dNdx, dNdy = np.linalg.inv(J) @ np.vstack([dksi, deta])
            waga = w[i] * w[j] * detJ
            H += k * (np.outer(dNdx, dNdx) + np.outer(dNdy, dNdy)) * waga
            N = N4(ksi, eta)
            C += rho_c * np.outer(N, N) * waga
    return H, C


def brzeg_elementu(xy, wezly_el, bc, alfa, t_ot, npc):
    """Hbc i P elementu z krawędzi 1-2, 2-3, 3-4, 4-1 (lokalnie 0-1, 1-2, 2-3, 3-0)."""
    p, w = gauss(npc)
    Hbc, P = np.zeros((4, 4)), np.zeros(4)
    for a in range(4):
        b = (a + 1) % 4
        if wezly_el[a] not in bc or wezly_el[b] not in bc:
            continue
        L = np.linalg.norm(xy[b] - xy[a])
        detJ = L / 2
        for s, ws in zip(p, w):
            N = np.zeros(4)
            N[a], N[b] = 0.5 * (1 - s), 0.5 * (1 + s)
            Hbc += alfa * np.outer(N, N) * ws * detJ
            P += alfa * N * t_ot * ws * detJ
    return Hbc, P


def rozwiaz(sciezka, npc=2, wypisz=True):
    gd, wezly, elementy, bc = wczytaj(sciezka)
    k, alfa, t_ot = gd["conductivity"], gd["alfa"], gd["tot"]
    rho_c = gd["density"] * gd["specificheat"]
    n = len(wezly)
    H, C, P = np.zeros((n, n)), np.zeros((n, n)), np.zeros(n)
    for el in elementy:                                   # agregacja
        xy = wezly[el]
        He, Ce = macierze_elementu(xy, k, rho_c, npc)
        Hbc, Pe = brzeg_elementu(xy, el, bc, alfa, t_ot, npc)
        idx = np.ix_(el, el)
        H[idx] += He + Hbc
        C[idx] += Ce
        P[el] += Pe
    dt = gd["simulationsteptime"]
    t = np.full(n, gd["initialtemp"])
    A = H + C / dt
    wyniki = []
    for krok in range(1, int(round(gd["simulationtime"] / dt)) + 1):
        t = np.linalg.solve(A, C / dt @ t + P)
        wyniki.append((krok * dt, t.min(), t.max()))
        if wypisz:
            print(f"{krok * dt:8.0f}  {t.min():12.6f}  {t.max():12.6f}")
    return H, C, P, wyniki


if __name__ == "__main__":
    plik = sys.argv[1] if len(sys.argv) > 1 else "../Kustra — siatki testowe/Test1_4_4.txt"
    npc = int(sys.argv[2]) if len(sys.argv) > 2 else 2
    print("  Czas[s]      MinTemp       MaxTemp")
    rozwiaz(plik, npc)
