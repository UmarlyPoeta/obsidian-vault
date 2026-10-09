---
typ: trenażer
tags: [rozmowa-gd, trenazer]
utworzono: 2026-10-09
cssclasses: [start]
---
# 🎯 Trenażer — symulacja rozmowy

> [!skrot] Jak używać (czytaj raz)
> 1. Wybierz temat i liczbę pytań → **🔀 Losuj**.
> 2. Przeczytaj pytanie i **odpowiedz NA GŁOS** (najlepiej po angielsku), jakbyś siedział na rozmowie. Max 1–2 minuty.
> 3. Dopiero potem rozwiń „Odpowiedź” i porównaj. Nie umiałeś → otwórz notatkę źródłową (link pod pytaniem) i przeczytaj sekcję.
> 4. Zapisz w dzienniku, które tematy kuleją → jutro zaczynasz od nich.
>
> Pytania są zbierane **automatycznie** ze wszystkich notatek w `Rozmowa GD/` (callouty „zadanie” + „odpowiedź”). Dopiszesz własne pytanie w dowolnej notatce — pojawi się tutaj samo.

```dataviewjs
const FOLDER = "Rozmowa GD";
const me = dv.current().file.path;
const pages = dv.pages(`"${FOLDER}"`).where(p => p.file.path !== me && p.typ === "pojęcie").sort(p => p.file.name);

// --- zbieranie pytań z calloutów [!zadanie] + [!odpowiedz] ---
const bank = [];
for (const p of pages) {
  const lines = (await dv.io.load(p.file.path)).split("\n");
  let cur = null, mode = null;
  const push = () => { if (cur && cur.a.length) bank.push(cur); cur = null; mode = null; };
  for (const line of lines) {
    const mq = line.match(/^> \[!zadanie\][-+]?\s*(.*)$/);
    if (mq) { push(); cur = { src: p.file.name, q: [mq[1]], a: [] }; mode = "q"; continue; }
    if (!cur) continue;
    const ma = line.match(/^> > \[!odpowiedz\][-+]?\s*(.*)$/);
    if (ma) { mode = "a"; const t = ma[1].trim(); if (t && !/^(Odpowiedź|Szkic|Rozwiązanie)$/i.test(t)) cur.a.push(`**${t}**`); continue; }
    if (mode === "a" && line.startsWith("> >")) { cur.a.push(line.replace(/^> > ?/, "")); continue; }
    if (mode === "q" && line.startsWith(">") && !line.startsWith("> >")) { cur.q.push(line.replace(/^> ?/, "")); continue; }
    if (!line.startsWith(">")) push();
  }
  push();
}

// --- UI ---
const bar = dv.el("div", "", { attr: { style: "display:flex;gap:.5em;flex-wrap:wrap;align-items:center;margin:.5em 0" } });
const sel = document.createElement("select");
const topics = ["🎲 Wszystkie tematy", ...new Set(bank.map(b => b.src))];
for (const t of topics) { const o = document.createElement("option"); o.value = t; o.textContent = t === topics[0] ? t : `${t} (${bank.filter(b => b.src === t).length})`; sel.appendChild(o); }
const num = document.createElement("input"); num.type = "number"; num.min = 1; num.max = 50; num.value = 8; num.style.width = "4em";
const btn = document.createElement("button"); btn.textContent = "🔀 Losuj"; btn.className = "mod-cta";
const info = document.createElement("span"); info.style.opacity = ".7";
bar.append(sel, num, btn, info);
const out = dv.el("div", "");

const toMd = (it, i) =>
  `> [!zadanie] ${i + 1}. ${it.q[0] || "Pytanie"}\n` +
  it.q.slice(1).map(l => `> ${l}`).join("\n") + (it.q.length > 1 ? "\n" : "") +
  `> *temat: [[${it.src}]]*\n>\n> > [!odpowiedz]- Odpowiedź\n` +
  it.a.map(l => `> > ${l}`).join("\n");

function draw() {
  out.innerHTML = "";
  let pool = sel.value === topics[0] ? bank : bank.filter(b => b.src === sel.value);
  pool = [...pool];
  for (let i = pool.length - 1; i > 0; i--) { const j = Math.floor(Math.random() * (i + 1)); [pool[i], pool[j]] = [pool[j], pool[i]]; }
  const pick = pool.slice(0, Math.max(1, parseInt(num.value) || 8));
  info.textContent = `${pick.length} z ${pool.length} pytań · bank: ${bank.length}`;
  pick.forEach((it, i) => out.appendChild(dv.el("div", toMd(it, i))));
}
btn.onclick = draw;
sel.onchange = draw;
draw();
```

## Tryb „rekruter” z Albertem (najskuteczniejszy)
Powiedz w terminalu: *„ej albert, zrób mi symulację rozmowy technicznej do Grid Dynamics — po angielsku, 10 pytań, po każdej mojej odpowiedzi oceń ją i dopytaj jak rekruter”*. Warianty:
- *„przepytaj mnie tylko z ROS 2 i Linuksa”*
- *„daj mi 3 scenariusze troubleshootingu i oceniaj moje kroki”*
- *„jesteś HR-owcem, zadaj mi pytania behawioralne i popraw mój angielski”*
- *„dopytuj mnie o mój projekt łodzi tak, jakbyś nie wierzył, że to ja zrobiłem”* ← bardzo ważne ćwiczenie!

## Fiszki (powtórka rozłożona w czasie)
- `Ctrl+Shift+R` → wybierz talię **rozmowa-gd** (podtalie: vla, teleop, ros2, tf, kinematyka, kamery, linux, sieci…).
- Oceniaj uczciwie: **Hard** = musiałem się zastanawiać, **Good** = od razu, **Easy** = banał.
- Codziennie do zera zaległych kart — 10–15 minut.

## Statystyki banku
```dataviewjs
const pages = dv.pages('"Rozmowa GD"').where(p => p.typ === "pojęcie").sort(p => p.file.name);
const rows = [];
for (const p of pages) {
  const t = await dv.io.load(p.file.path);
  const q = (t.match(/^> \[!zadanie\]/gm) || []).length;
  const f = (t.split(/^## Fiszki/m)[1] || "").split(/^## /m)[0].split("\n").filter(l => l.includes("::")).length;
  rows.push([p.file.link, p.status ?? "—", q, f]);
}
dv.table(["Notatka", "Status", "Pytania", "Fiszki"], rows);
```
