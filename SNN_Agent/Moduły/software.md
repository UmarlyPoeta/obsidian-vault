---
typ: snn-modul
sciezka: software/
tags: [snn, modul]
zaktualizowano: 2026-10-05
---
# software

> [!skrot] W 30 sekund
> - **Co to:** szkielet z restrukturyzacji repo (commit 006797e3, 2026-02-08, vityk-dev, „Restructure project into hardware/software/firmware layers”). Zawiera mock lokalnego agenta LLM z RAG (`host_agent`), pusty backend FastAPI, szablon Vite + React oraz docker-compose. Od tamtej pory nic tu nie zmieniano.
> - **Wejście → wyjście:** w praktyce brak. Wszystkie pliki to placeholdery typu „hello world”.
> - **Gdzie działa:** nigdzie produkcyjnie. Aktualny edge i backend leżą w [[rpi_agents]], a dashboard na gałęzi `feat/karolina-dashboard`.

## Struktura (drzewo z opisem plików)
```
software/
├── host_agent/
│   ├── run.py                 # infer(prompt) → f"[mock] {prompt}"
│   ├── .gitignore             # models/, *.gguf, *.bin, *.pt, *.safetensors
│   └── rag/
│       ├── ingest.py          # chromadb.PersistentClient("chroma"), dodaje 1 dokument "hello world"
│       ├── query.py           # zapytanie "hello", n_results=1
│       └── requirements.txt   # langchain==0.3.0, chromadb==0.5.5, pydantic>=2
├── web_dashboard/
│   ├── api/app.py             # FastAPI(title="IDS Backend"), GET /health → {"status":"ok"}
│   ├── api/requirements.txt   # fastapi==0.115.0, uvicorn[standard]==0.30.0
│   └── ui/                    # szablon Vite + React 19 + TS (App.tsx = licznik z demo Vite)
└── infra/
    ├── docker-compose.yml     # mongo:7, chromadb/chroma, backend, frontend
    └── dockerfiles/{backend,frontend}.Dockerfile
```

## Jak działa
- `host_agent` wskazuje na zamiar lokalnego LLM (ignorowane `*.gguf`) z RAG w ChromaDB. Kod to wyłącznie mock (`run.py:1-4`).
- `web_dashboard/api/app.py` udostępnia jeden endpoint `/health`.
- `infra/docker-compose.yml` buduje `../backend` i `../frontend`. **Takich katalogów nie ma** (są `web_dashboard/api` i `web_dashboard/ui`). W usłudze `frontend` klucz `dockerfile:` ma złe wcięcie i leży poza `build:`.

## Jak uruchomić
Brak instrukcji w repo. Teoretycznie `python software/host_agent/run.py` (wypisze `[mock] Hello IDS`). `docker compose` w obecnym stanie nie zbuduje się (zob. wyżej).

## Konfiguracja i parametry
Porty z compose: mongo 27017, chroma 8001→8000, backend 8000, frontend 5173.

## Wyniki
Brak.

## Powiązania
- Zastąpione przez: [[rpi_agents]] (bridge edge, backend `cloud/app`, mock API) oraz dashboard Karoliny (gałąź `feat/karolina-dashboard`, `rpi_agents/cloud/app/static`). Kontekst: [[Sprzęt i wdrożenie]], [[Dokumentacja projektu]].

## Na gałęziach
`software/` w tej samej postaci występuje na `feat/rpi` i `feat/dashboard` (ich drzewa mają katalog `software`). Według `git log -- software` jedynym commitem jest 006797e3. **niepewne:** czy jakakolwiek gałąź rozwija `host_agent`. `git log` na gałęzi bieżącej tego nie pokazuje, a innych gałęzi pod tym kątem nie przeglądałem.

## Znane problemy / TODO
- Martwy szkielet: compose wskazuje na nieistniejące ścieżki, frontend to nietknięty szablon Vite.
- Pr-gate (`.github/workflows/pr.yml`) nie obejmuje `software/`.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
