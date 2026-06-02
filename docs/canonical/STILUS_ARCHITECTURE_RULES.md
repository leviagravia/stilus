# STILUS — Architecture Rules

Version: Canonical Simplex v1

## Purpose

Questo documento definisce le regole architetturali non negoziabili di Stilus.

Ogni modifica al codice deve essere compatibile con queste regole.

## Fundamental Principle

La semplicità architetturale è un requisito funzionale.

Una soluzione più semplice è preferibile a una più complessa quando produce lo stesso risultato per l'utente.

## Architectural Goal

Massimo valore editoriale.

Minima complessità tecnica.

## Native Desktop

Stilus è un'applicazione desktop nativa.

Non è una web application.

Non è una shell browser.

## Approved Technologies

### UI

- GTK

### Language

- C

### Data

- Plain Text
- Markdown

### Encoding

- UTF-8

## Rejected Technologies

### Runtime Layers

- Electron
- Chromium Shells
- Browser UI Frameworks

### AI

- LLM
- Copilot
- AI Assistant
- Embedded AI Services

### Cloud

- Cloud Sync
- Collaboration Servers
- Remote Storage Dependency

## Document First Architecture

L'unità centrale del sistema è il documento.

Non:

- workspace
- project
- database
- session

## Persistence Model

Il file sul disco è la fonte di verità.

Stilus non deve introdurre una dipendenza obbligatoria da database.

## Accepted Persistence

- TXT
- MD
- Backup Files
- Recovery Files

## Rejected Persistence

- Mandatory SQLite
- Mandatory Internal Database
- Proprietary Project Containers

## Window Model

Relazione canonica:

Document
→ Tab
→ Window

Non:

Document
→ Pane
→ Workspace
→ Layout Engine

## Multiple Windows

Supportate.

Ogni finestra è indipendente.

## Split Views

Respinte.

Non fanno parte dell'architettura.

Motivo:

La moltiplicazione dei pannelli aumenta la complessità senza beneficio proporzionale per la scrittura.

## Component Responsibility

Ogni componente deve avere una responsabilità chiara.

Esempio:

Editor Buffer
→ editing

Status Bar
→ informazioni

Spell Checker
→ controllo ortografico

Non devono esistere componenti con responsabilità miste.

## Feature Admission Rule

Una nuova feature può entrare in Stilus solo se:

1. migliora concretamente la scrittura;
2. non introduce complessità sproporzionata;
3. non altera l'identità del prodotto.

## Feature Rejection Rule

Una feature deve essere respinta se:

- aggiunge complessità elevata;
- richiede infrastrutture dedicate;
- trasforma Stilus in altro prodotto.

## Plugin Policy

Nessun runtime plugin.

Nessun marketplace.

Nessun sistema di estensioni dinamiche.

Motivo:

Ridurre complessità, instabilità e frammentazione.

## Testing Policy

Prima della patch:

- analisi del problema
- identificazione causa
- verifica

Dopo la patch:

- test
- verifica
- conferma

Patch speculative proibite.

## Engineering Rule

Mai correggere un sintomo.

Correggere la causa.

## UI Rule

Ogni elemento visibile deve avere uno scopo chiaro.

UI ornamentale da evitare.

## Status Information Rule

Gli stati importanti devono essere espliciti.

Consentito:

Saved

Unsaved

Writable

Protected

Proibito:

- indicatori nascosti
- simboli ambigui
- stati impliciti

## Complexity Budget

Ogni nuova feature consuma budget architetturale.

Se il costo è elevato:

- spostarla in Stilus Plus
oppure
- spostarla in Stilus Ultra

## Canonical Exclusions

- AI
- Cloud
- Collaboration
- Electron
- IDE Features
- LSP
- Debuggers
- Terminals
- Workspace Managers
- Project Trees
- Plugin Systems

## Final Rule

Quando esiste un conflitto tra:

- semplicità
e
- complessità

vince la semplicità.

