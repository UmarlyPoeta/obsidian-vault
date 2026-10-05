import math, os
import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

OUT = "/home/umarly-poeta/projects/obsidian-vault/Załączniki/Solar/Regulator"
os.makedirs(OUT, exist_ok=True)
plt.rcParams.update({"figure.dpi": 110, "font.size": 11, "axes.grid": True,
                     "grid.alpha": 0.3, "figure.facecolor": "white"})
C_N, C_Z, C_P = "#d62728", "#7f7f7f", "#2ca02c"

def save(fig, name):
    fig.tight_layout()
    fig.savefig(os.path.join(OUT, name), facecolor="white")
    plt.close(fig)

def sat(x, lim=1.0):
    return np.clip(x, -lim, lim)

def wrap(a):
    return math.atan2(math.sin(a), math.cos(a))

# ---------- FIS (1:1 z code_gen_1.c) ----------
def tri(x, a, b, c):
    x = np.asarray(x, dtype=float)
    y = np.zeros_like(x)
    m = (a < x) & (x < b); y[m] = (x[m] - a) / (b - a)
    m = (b < x) & (x < c); y[m] = (c - x[m]) / (c - b)
    y[x == b] = 1
    return y

def mf_in(v, s2):
    return {"N": float(tri([v], -2, -1, 0)[0]), "Z": math.exp(-v * v / s2),
            "P": float(tri([v], 0, 1, 2)[0])}

XS = np.linspace(-1, 1, 101)
OUTMF = {"N": tri(XS, -2, -1, 0), "Z": tri(XS, -1, 0, 1), "P": tri(XS, 0, 1, 2)}
RULES = [  # (wejscie, MF, lewy, prawy)
    ("e_v", "N", "N", "N"), ("e_v", "Z", "Z", "Z"), ("e_v", "P", "P", "P"),
    ("e_w", "Z", "Z", "Z"), ("e_w", "N", "P", "N"), ("e_w", "P", "N", "P"),
    ("e_end", "Z", "Z", "Z"), ("e_end", "N", "P", "N"), ("e_end", "P", "N", "P"),
]

def fis(e_v, e_w, e_end, verbose=False):
    m = {"e_v": mf_in(e_v, 0.18), "e_w": mf_in(e_w, 0.18), "e_end": mf_in(e_end, 0.08)}
    w = [m[inp][k] for inp, k, _, _ in RULES]
    aggs = []
    for side in (2, 3):
        agg = np.zeros(101)
        for wi, r in zip(w, RULES):
            agg = np.maximum(agg, np.minimum(OUTMF[r[side]], wi))
        aggs.append(agg)
    cents = [(a * XS).sum() / a.sum() for a in aggs]
    if verbose:
        for r, wi in zip(RULES, w):
            print(f"  R {r[0]:5s} {r[1]} -> L {r[2]} R {r[3]}  waga {wi:.3f}")
    return cents, aggs, m, w

# ---------- 1. zawijanie kata ----------
fig, ax = plt.subplots(1, 2, figsize=(11, 4))
h = np.linspace(-540, 540, 2000)
raw = -h * math.pi / 180
ax[0].plot(h, raw, color="#1f77b4", label="heading_raw = −heading·π/180")
ax[0].plot(h, [wrap(a) for a in raw], color="#ff7f0e", lw=2, label="ψ = atan2(sin, cos)  (zawinięty)")
ax[0].axhline(math.pi, ls="--", c="k", lw=0.8); ax[0].axhline(-math.pi, ls="--", c="k", lw=0.8)
ax[0].set_xlabel("heading z /filter/euler [°]"); ax[0].set_ylabel("[rad]")
ax[0].set_title("Stopnie → radiany i zawinięcie do ±π"); ax[0].legend(fontsize=9)
t = np.linspace(0, 2 * math.pi, 200)
ax[1].plot(np.cos(t), np.sin(t), c="#999")
for deg, col in [(30, "#2ca02c"), (390, "#d62728")]:
    a = math.radians(deg)
    ax[1].annotate("", xy=(math.cos(a), math.sin(a)), xytext=(0, 0),
                   arrowprops=dict(arrowstyle="->", color=col, lw=2))
ax[1].text(0.95, 0.62, "30° i 390°\nto ten sam kierunek", fontsize=10)
ax[1].set_aspect("equal"); ax[1].set_xlim(-1.3, 1.9); ax[1].set_ylim(-1.3, 1.3)
ax[1].set_title("Po co zawijać: kąt jest „okrągły”")
save(fig, "01_kat_kursu.png")

# ---------- 2. geometria pure pursuit ----------
fig, ax = plt.subplots(figsize=(8, 6))
path = np.array([[-1, 1.0], [8, 1.0]])
ax.plot(path[:, 0], path[:, 1], "k-", lw=3, label="ścieżka (/path_points)")
boat = np.array([0.0, 0.0]); psi = 0.0; L = 2.0
proj = np.array([0.0, 1.0])
la = np.array([math.sqrt(L**2 - 1) if L > 1 else 1.0, 1.0])
la = proj + np.array([L, 0])  # PP liczy L wzdłuż ścieżki od rzutu
ax.plot(*boat, "o", ms=12, c="#1f77b4")
ax.annotate("", xy=boat + 1.4 * np.array([math.cos(psi), math.sin(psi)]), xytext=boat,
            arrowprops=dict(arrowstyle="-|>", color="#1f77b4", lw=2.5))
ax.text(1.45, -0.25, "dziób (ψ)", color="#1f77b4")
ax.plot(*proj, "s", ms=9, c="#9467bd"); ax.text(-0.9, 1.25, "rzut łodzi\nna ścieżkę", color="#9467bd")
ax.annotate("", xy=la, xytext=proj, arrowprops=dict(arrowstyle="<->", color="#9467bd"))
ax.text(0.7, 1.15, "L (lookahead)\nmierzone wzdłuż ścieżki", color="#9467bd", fontsize=9)
ax.plot(*la, "*", ms=18, c="#ff7f0e"); ax.text(la[0] + 0.15, la[1] + 0.15, "punkt lookahead", color="#ff7f0e")
ax.plot([boat[0], la[0]], [boat[1], la[1]], "--", c="#ff7f0e")
alpha = wrap(math.atan2(la[1] - boat[1], la[0] - boat[0]) - psi)
arc = np.linspace(0, alpha, 30)
ax.plot(0.9 * np.cos(arc), 0.9 * np.sin(arc), c="#d62728", lw=2)
ax.text(0.95, 0.15, f"α = {math.degrees(alpha):.0f}°", color="#d62728", fontsize=12)
d = math.hypot(*(la - boat)); R = d / (2 * math.sin(alpha))
th = np.linspace(-math.pi / 2, -math.pi / 2 + 2 * alpha, 60)
ax.plot(R * np.cos(th), R + R * np.sin(th), c="#2ca02c", lw=2, label=f"łuk, którym łódź dojedzie (R = {R:.2f} m)")
ax.set_aspect("equal"); ax.set_xlim(-1.5, 5); ax.set_ylim(-1.2, 3)
ax.set_title("Pure Pursuit: „goń marchewkę oddaloną o L”")
ax.legend(loc="upper right", fontsize=9)
save(fig, "02_pure_pursuit_geometria.png")

# ---------- 3. lookahead i omega vs alpha ----------
a = np.linspace(0, math.pi, 400)
Lh = 1 - np.minimum(1, np.abs(0.7 * np.arctan(2 * a)))
w = np.clip(2 * np.sin(a) / np.maximum(Lh, 1e-6), -1, 1)
w_free = 2 * np.sin(a) / np.maximum(Lh, 1e-6)
fig, ax = plt.subplots(1, 2, figsize=(11, 4))
ax[0].plot(np.degrees(a), Lh, c="#9467bd", lw=2)
ax[0].set_xlabel("α = błąd kierunku [°]"); ax[0].set_ylabel("L [m]  (= też v zadane [m/s])")
ax[0].set_title("Lookahead kurczy się przy dużym błędzie\n(desired_vel = 1)")
ax[0].annotate("prosto: L = 1 m", xy=(0, 1), xytext=(40, 0.9), arrowprops=dict(arrowstyle="->"))
ax[0].annotate("90°: L ≈ 0.12 m", xy=(90, 0.116), xytext=(100, 0.4), arrowprops=dict(arrowstyle="->"))
ax[1].plot(np.degrees(a), np.minimum(w_free, 6), c="#bbb", ls="--", label="2·sin α / L (bez limitu)")
ax[1].plot(np.degrees(a), w, c="#d62728", lw=2, label="ω_ref po nasyceniu ±1 rad/s")
ax[1].axvline(np.degrees(a[np.argmax(w >= 0.999)]), c="k", ls=":", lw=1)
ax[1].text(np.degrees(a[np.argmax(w >= 0.999)]) + 3, 4.6, f"nasycenie od\n~{np.degrees(a[np.argmax(w >= 0.999)]):.0f}°")
ax[1].set_xlabel("α [°]"); ax[1].set_ylabel("ω [rad/s]"); ax[1].set_ylim(0, 6)
ax[1].set_title("Zadana prędkość kątowa"); ax[1].legend(fontsize=9, loc="center right")
save(fig, "03_lookahead_omega.png")

# ---------- 4. nasycenie i tanh ----------
x = np.linspace(-4, 4, 400)
fig, ax = plt.subplots(1, 3, figsize=(13, 3.8))
ax[0].plot(x, x, c="#bbb", ls="--", label="x"); ax[0].plot(x, sat(x), c="#1f77b4", lw=2, label="sat(x) do ±1")
ax[0].set_title("Saturacja (obcięcie)"); ax[0].legend(fontsize=9); ax[0].set_ylim(-2.5, 2.5)
ax[1].plot(x, np.tanh(x), c="#ff7f0e", lw=2, label="tanh(x)")
ax[1].plot(x, sat(x), c="#1f77b4", ls="--", label="sat(x)")
ax[1].set_title("tanh = „miękka” saturacja"); ax[1].legend(fontsize=9)
ax[2].plot(x, np.tanh(0.5 * x), c="#2ca02c", lw=2, label="v: tanh(0.5·x_loc)")
ax[2].plot(x, np.clip(np.tanh(x), -0.5, 0.5), c="#d62728", lw=2, label="ω: sat₀.₅(tanh(y_loc))")
ax[2].set_xlabel("odległość do końca w osi łodzi [m]")
ax[2].set_title("Hamulce przy końcu ścieżki (d ≤ 1 m)"); ax[2].legend(fontsize=9)
save(fig, "04_saturacja_tanh.png")

# ---------- 5. hamulce: lokalne x,y ----------
fig, ax = plt.subplots(figsize=(6.5, 5))
ax.plot(0, 0, "o", ms=12, c="#1f77b4"); ax.annotate("", xy=(1.3, 0), xytext=(0, 0), arrowprops=dict(arrowstyle="-|>", lw=2.5, color="#1f77b4"))
ax.annotate("", xy=(0, 1.3), xytext=(0, 0), arrowprops=dict(arrowstyle="-|>", lw=1.5, color="#7f7f7f"))
ax.text(1.35, -0.12, "x_loc (przód)"); ax.text(0.05, 1.35, "y_loc (lewo/prawo)")
ax.plot(0.7, 0.5, "*", ms=18, c="#ff7f0e"); ax.text(0.75, 0.55, "koniec ścieżki")
ax.plot([0, 0.7], [0, 0], ":", c="#2ca02c", lw=2); ax.plot([0.7, 0.7], [0, 0.5], ":", c="#d62728", lw=2)
ax.text(0.25, -0.15, "x_loc = 0.7", color="#2ca02c"); ax.text(0.73, 0.2, "y_loc = 0.5", color="#d62728")
ax.set_aspect("equal"); ax.set_xlim(-0.5, 2); ax.set_ylim(-0.5, 1.6)
ax.set_title("Rzut końca ścieżki na osie łodzi\nv ∝ tanh(0.5·0.7) = 0.34,  ω = tanh(0.5) = 0.46")
save(fig, "05_hamulce_osie_lodzi.png")

# ---------- 6. MF wejsc ----------
v = np.linspace(-1.5, 1.5, 600)
fig, ax = plt.subplots(1, 2, figsize=(11, 3.8))
for i, (s2, name) in enumerate([(0.18, "e_v i e_ω (σ = 0.3)"), (0.08, "e_end (σ = 0.2)")]):
    ax[i].plot(v, tri(v, -2, -1, 0), c=C_N, lw=2, label="N (ujemny)")
    ax[i].plot(v, np.exp(-v**2 / s2), c=C_Z, lw=2, label="Z (około zera)")
    ax[i].plot(v, tri(v, 0, 1, 2), c=C_P, lw=2, label="P (dodatni)")
    ax[i].axvspan(-1.5, -1, color="#eee"); ax[i].axvspan(1, 1.5, color="#eee")
    ax[i].set_title(f"Funkcje przynależności: {name}"); ax[i].set_xlabel("błąd (po saturacji do ±1)")
    ax[i].set_ylabel("stopień przynależności μ"); ax[i].legend(fontsize=9)
ax[0].plot([0.3, 0.3], [0, 1], "k:"); ax[0].text(0.32, 0.85, "e = 0.3:\nZ = 0.61\nP = 0.30", fontsize=9)
save(fig, "06_mf_wejscia.png")

# ---------- 7. MF wyjsc ----------
fig, ax = plt.subplots(figsize=(7, 3.6))
for k, c in [("N", C_N), ("Z", C_Z), ("P", C_P)]:
    ax.plot(XS, OUTMF[k], c=c, lw=2, label=f"{k}")
ax.set_xlabel("ciąg silnika (zanim pomnożymy ×15000)"); ax.set_ylabel("μ")
ax.set_title("Funkcje przynależności wyjścia (lewy = prawy)\nN = wstecz, Z = stój, P = naprzód"); ax.legend()
save(fig, "07_mf_wyjscia.png")

# ---------- 8. przyklad defuzyfikacji ----------
def plot_defuzz(e_v, e_w, e_end, name, title):
    cents, aggs, m, w = fis(e_v, e_w, e_end)
    fig, ax = plt.subplots(1, 2, figsize=(11, 3.8))
    for i, side in enumerate(["LEWY", "PRAWY"]):
        ax[i].fill_between(XS, aggs[i], color="#1f77b4", alpha=0.35)
        ax[i].plot(XS, aggs[i], c="#1f77b4")
        ax[i].axvline(cents[i], c="#d62728", lw=2)
        thr = float(np.clip(15 * cents[i], -1, 1) * 1000)
        ax[i].text(cents[i] + 0.03, 0.9, f"środek ciężkości = {cents[i]:+.3f}\n×15000 → {thr:+.0f}", color="#d62728")
        ax[i].set_title(f"{side}: zagregowany kształt"); ax[i].set_ylim(0, 1.1)
    fig.suptitle(title)
    save(fig, name)
    return cents

# ---------- 9. ciag vs blad ----------
ew = np.linspace(-0.6, 0.6, 121)
fig, ax = plt.subplots(1, 2, figsize=(11, 4))
for g, ls in [(15, "-"), (5, "--")]:
    l = [np.clip(g * fis(0, float(sat(2 * e)), 0)[0][0], -1, 1) * 1000 for e in ew]
    ax[0].plot(ew, l, ls=ls, lw=2, label=f"lewy, wzmocnienie {g}")
ax[0].set_xlabel("błąd prędkości kątowej [rad/s]"); ax[0].set_ylabel("ciąg lewego silnika")
ax[0].set_title("Obrót: przy ×15 to prawie przekaźnik"); ax[0].legend(fontsize=9)
ev = np.linspace(-1.2, 1.2, 121)
for g, ls in [(15, "-"), (5, "--")]:
    l = [np.clip(g * fis(float(sat(e)), 0, 0)[0][0], -1, 1) * 1000 for e in ev]
    ax[1].plot(ev, l, ls=ls, lw=2, label=f"oba silniki, wzmocnienie {g}")
ax[1].set_xlabel("błąd prędkości liniowej [m/s]"); ax[1].set_title("Jazda naprzód/wstecz"); ax[1].legend(fontsize=9)
save(fig, "09_ciag_vs_blad.png")

# ---------- 10. zabawkowa symulacja: schodki vs prosta, L maly vs duzy ----------
def stairs():
    pts = [(0, 0)]
    x = y = 0.0
    for i in range(12):
        x += 2.0; pts.append((x, y))
        y += 1.0; x += 1.0; pts.append((x, y))
    return np.array(pts)

def lookahead_pt(path, pos, L):
    # rzut na najblizszy segment + odmierzenie L wzdluz lamanej
    best = (1e9, 0, path[0])
    for i in range(len(path) - 1):
        a, b = path[i], path[i + 1]; ab = b - a
        t = np.clip(np.dot(pos - a, ab) / np.dot(ab, ab), 0, 1)
        p = a + t * ab; d = np.linalg.norm(pos - p)
        if d < best[0]: best = (d, i, p)
    _, i, p = best
    rem = L
    while i < len(path) - 1:
        seg = np.linalg.norm(path[i + 1] - p)
        if seg >= rem:
            return p + (path[i + 1] - p) / seg * rem
        rem -= seg; p = path[i + 1]; i += 1
    return path[-1]

def sim(path, mode, T=40, dt=0.02, tau=0.8):
    pos = np.array([0.0, -0.8]); psi = 0.0; w = 0.0; a_prev = 0.0; vd = 1.0
    xs = [pos.copy()]
    for _ in range(int(T / dt)):
        if mode == "orig":
            L = max(1e-3, vd * (1 - min(1, abs(0.7 * math.atan(2 * a_prev)))))
        else:
            L = 4.0
        lp = lookahead_pt(path, pos, L)
        alpha = wrap(math.atan2(lp[1] - pos[1], lp[0] - pos[0]) - psi)
        a_prev = alpha
        wcmd = float(np.clip(2 * math.sin(alpha) / L, -1, 1))
        if mode == "orig":  # przekaznik jak FIS x15: pelny skret przy duzym bledzie
            wcmd = float(np.clip(5 * (wcmd - w), -1, 1)) + w
        w += (wcmd - w) * dt / tau
        v = vd * (L if mode == "orig" else 1.0)
        psi = wrap(psi + w * dt)
        pos = pos + v * dt * np.array([math.cos(psi), math.sin(psi)])
        xs.append(pos.copy())
        WS.setdefault(mode, []).append(w)
    return np.array(xs)

WS = {}

P = stairs()
fig, ax = plt.subplots(1, 2, figsize=(13, 4.5), sharey=True)
for i, (mode, ttl) in enumerate([("orig", "Teraz: L ≤ 1 m i maleje z α"), ("fix", "Propozycja: stałe L = 4 m")]):
    tr = sim(P, mode)
    ax[i].plot(P[:, 0], P[:, 1], "k-o", ms=3, lw=1.5, label="schodkowa ścieżka z A*")
    ax[i].plot(tr[:, 0], tr[:, 1], c="#d62728" if mode == "orig" else "#2ca02c", lw=2, label="łódź")
    ax[i].set_title(ttl); ax[i].set_aspect("equal"); ax[i].legend(fontsize=9, loc="upper left")
    ax[i].set_xlim(-1, 22); ax[i].set_ylim(-2, 9)
fig.suptitle("Zabawkowa symulacja (prosty model łodzi z bezwładnością obrotu τ = 0.8 s), tylko poglądowo")
save(fig, "10_symulacja_schodki.png")

fig, ax = plt.subplots(figsize=(12, 3.6))
for mode, col, lab in [("orig", "#d62728", "teraz (L ≤ 1 m)"), ("fix", "#2ca02c", "stałe L = 4 m")]:
    t_ = np.arange(len(WS[mode])) * 0.02
    ax.plot(t_, WS[mode], c=col, lw=1.8, label=lab)
ax.axhline(0, c="k", lw=0.8)
ax.set_xlabel("czas [s]"); ax.set_ylabel("prędkość obrotu ω [rad/s]")
ax.set_title("Ta sama symulacja: jak bardzo łódź „kręci” w czasie. Czerwone = lewo-prawo-lewo-prawo")
ax.legend(fontsize=9)
save(fig, "10b_symulacja_omega.png")

# ---------- 11. sciezka: schodki vs line-of-sight ----------
fig, ax = plt.subplots(1, 2, figsize=(12, 4.2), sharey=True)
grid = [(i, j) for i in range(0, 15) for j in range(0, 7)]
for a_ in ax:
    a_.scatter(*zip(*grid), s=4, c="#ccc")
    a_.add_patch(plt.Rectangle((6, 1.5), 2, 3, color="#555"))
    a_.text(6.3, 4.7, "przeszkoda", fontsize=9)
    a_.set_aspect("equal")
astar = np.array([(0, 0), (1, 1), (2, 1), (3, 1), (4, 1), (5, 1), (6, 1), (7, 1), (8, 1), (9, 2), (10, 3), (11, 3), (12, 4), (13, 5), (14, 6)])
corners = astar[[0, 1, 8, 10, 11, 12, 14]]
ax[0].plot(astar[:, 0], astar[:, 1], c="#1f77b4", lw=1)
ax[0].plot(corners[:, 0], corners[:, 1], "o", c="#d62728", ms=8, label=f"/path_points: {len(corners)} załamań")
ax[0].set_title("Teraz: punkt przy każdej zmianie kierunku o 45°"); ax[0].legend(fontsize=9, loc="upper left")
los = np.array([(0, 0), (6, 1), (8, 1), (14, 6)])
ax[1].plot(astar[:, 0], astar[:, 1], c="#bbb", lw=1, ls="--", label="surowe A*")
ax[1].plot(los[:, 0], los[:, 1], "-o", c="#2ca02c", lw=2.5, ms=8, label=f"line-of-sight: {len(los)} punkty")
ax[1].set_title("Propozycja: tylko punkty potrzebne do ominięcia"); ax[1].legend(fontsize=9, loc="upper left")
save(fig, "11_sciezka_schodki_vs_los.png")

# ---------- przyklad liczbowy ----------
print("== PRZYKLAD: lodz (0,0), psi=0, sciezka y=1, v_d=1, poprzednie alfa=0, v=1, omega_imu=0")
L = 1.0
lp = np.array([1.0, 1.0])
alpha = math.atan2(1, 1)
wpp = 2 * math.sin(alpha) / L
print(f"alpha={alpha:.4f} rad ({math.degrees(alpha):.1f} deg) omega_pp={wpp:.3f} -> sat {min(1, wpp)}")
e_v = 1.0 - 1.0; e_w = float(sat(2 * (min(1, wpp) + 0)))
print(f"e_v={e_v} e_w={e_w}")
cents, aggs, m, w = fis(e_v, e_w, 0, verbose=True)
print("mf", m)
print("centroidy", cents, "ciag", [float(np.clip(15 * c, -1, 1) * 1000) for c in cents])
Lnext = 1 - abs(0.7 * math.atan(2 * alpha))
print(f"nastepny krok: Delay={1-Lnext:.3f}, L={Lnext:.3f}")
plot_defuzz(e_v, e_w, 0, "08_defuzyfikacja_przyklad.png", "Przykład: e_v = 0, e_ω = +1 (trzeba mocno skręcić)")
print("== PRZYKLAD 2: maly blad e_w = 0.2 (omega err 0.1)")
c2, *_ = fis(0, 0.2, 0, verbose=True)
print(c2, [float(np.clip(15 * c, -1, 1) * 1000) for c in c2])
plot_defuzz(0, 0.2, 0, "08b_defuzyfikacja_maly_blad.png", "Przykład: e_v = 0, e_ω = +0.2 (lekka korekta)")
print("== mf przy 0.3", mf_in(0.3, 0.18))
