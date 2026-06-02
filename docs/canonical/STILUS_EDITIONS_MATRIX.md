# STILUS — Editions Matrix

Version: Canonical Simplex v1

## Purpose

Questo documento definisce la separazione ufficiale delle funzionalità tra:

- Stilus
- Stilus Plus
- Stilus Ultra

La divisione non è commerciale.

La divisione è esclusivamente tecnica.

Ogni funzione viene collocata nell'edizione più appropriata in base a:

- complessità implementativa
- costo di manutenzione
- impatto architetturale
- beneficio editoriale

---

# Philosophy

Stilus deve restare semplice.

Le funzionalità che aumentano significativamente la complessità non devono entrare nell'edizione base.

La regola è:

beneficio elevato + costo basso
=
Stilus

beneficio elevato + costo medio
=
Stilus Plus

beneficio elevato + costo alto
=
Stilus Ultra

---

# STILUS (Simplex)

## Mission

Editor editoriale e accademico essenziale.

Alta densità editoriale.

Bassa complessità tecnica.

## Core Editing

- New
- Open
- Save
- Save As
- Reload
- Recent Files
- Multiple Windows
- Drag and Drop

## Editing

- Undo
- Redo
- Cut
- Copy
- Paste
- Select All

## Search

- Find
- Find Next
- Find Previous
- Replace
- Go To Line

## Writing

- Word Count
- Character Count
- Reading Time
- Session Statistics
- Focus Mode
- Fullscreen
- Typewriter Mode

## Language

- Spell Checking
- Language Selection

## Formats

- TXT
- Markdown

## Safety

- Autosave
- Recovery
- Backup Copies

## Status Bar

- Saved / Unsaved
- Writable / Protected
- Language
- Word Count
- Line / Column
- UTF-8
- TXT / MD
- File Size

## Explicit Exclusions

- Projects
- Workspace
- Cloud
- AI
- Plugins
- Collaboration
- Split Views

---

# STILUS PLUS

## Mission

Funzionalità avanzate utili agli scrittori che non alterano l'identità del prodotto.

## Advanced Writing

- Named Bookmarks
- Persistent Bookmarks
- Writing Targets
- Daily Goals
- Session Goals
- Writing Streaks

## Navigation

- Markdown Outline
- Heading Navigator
- Document Map

## Academic Writing

- Notes Panel
- References Panel
- Citation Helpers

## Export

- HTML
- PDF
- EPUB

## Revision

- Snapshot History
- Snapshot Restore
- Compare Snapshot

## Search

- Multi-file Search

## Additional Statistics

- Sentence Count
- Paragraph Count
- Readability Metrics

---

# STILUS ULTRA

## Mission

Funzioni potenti ma tecnicamente costose.

Da mantenere isolate dall'edizione base.

## Advanced Revision

- Visual Diff
- Side-by-Side Comparison
- Multi-Version Review

## Academic Features

- Citation Database
- Bibliography Manager
- Cross References

## Publishing

- Advanced EPUB Pipeline
- Advanced PDF Pipeline

## Structural Navigation

- Large Document Navigator
- Multi-document Collections

## Analysis

- Style Analysis
- Repetition Detection
- Terminology Consistency Checks

## Long Form Writing

- Book Structure Manager
- Chapter Collections
- Manuscript Navigator

---

# Features Never Allowed

Queste funzionalità non appartengono a nessuna edizione.

## AI

- Chatbots
- Assistants
- Generative Writing
- LLM Integration

## Cloud

- Cloud Sync
- Cloud Storage
- Online Accounts

## Collaboration

- Multi-user Editing
- Real-time Collaboration

## IDE Features

- LSP
- Debuggers
- Terminals
- Build Systems

## Platform Features

- Plugin Marketplace
- Extension Ecosystem

## Electron

- Electron Runtime
- Chromium Shell

---

# Canonical Rule

Quando una nuova funzionalità viene proposta:

1. valutare il beneficio editoriale;
2. valutare il costo tecnico;
3. assegnare l'edizione corretta.

Se esiste dubbio:

la funzione deve essere collocata nell'edizione superiore.

