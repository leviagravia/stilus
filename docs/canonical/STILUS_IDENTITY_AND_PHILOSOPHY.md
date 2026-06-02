# STILUS — Identity and Philosophy

Version: Canonical Simplex v1

## Identity

Stilus è un editor desktop nativo dedicato alla scrittura.

È progettato per:

- scrittura editoriale;
- academic writing;
- long-form writing;
- revisione testuale;
- editing e correzione;
- produzione di documenti in plain text e Markdown.

Stilus non è un ambiente di sviluppo.

Stilus non è un sistema di gestione progetti.

Stilus non è una piattaforma estendibile.

Stilus non è un word processor WYSIWYG.

Stilus non è un prodotto AI.

## Mission

Fornire il massimo supporto possibile alla scrittura mantenendo il minimo livello possibile di complessità architetturale.

La priorità assoluta è la qualità dell'esperienza di scrittura.

La complessità tecnica deve essere giustificata da un beneficio concreto per il processo di scrittura.

## Formula Identitaria

Stilus deve rimanere:

"un editor evoluto per scrittura editoriale e academic writing con alta densità editoriale e bassa complessità architetturale"

## Non Goals

Stilus non deve diventare:

- IDE
- Code Editor
- Knowledge Base
- PKM
- Workspace Manager
- Project Manager
- Office Suite
- CMS
- Database Documentale
- AI Assistant
- Cloud Platform

## Core Values

### Simplicity

La semplicità è una funzionalità.

Ogni nuova feature deve giustificare il proprio costo architetturale.

### Stability

La stabilità è preferita alla velocità di sviluppo.

### Predictability

L'utente deve sapere sempre cosa sta succedendo.

Sono preferiti indicatori espliciti rispetto a simboli impliciti.

Esempio:

Corretto:

Saved
Unsaved

Errato:

*
•
badge nascosti

### Native Desktop First

Stilus è un'applicazione desktop nativa.

Non è una web application.

Non è una shell Electron.

## Mandatory Exclusions

Le seguenti categorie sono escluse dal progetto:

- AI
- LLM
- Copilot
- Cloud Sync
- Collaboration
- Electron
- Plugin Runtime
- Extension Marketplace
- Workspace Engine
- IDE Features
- LSP Dependency
- Terminal Integration
- Debugger Integration

## Document Philosophy

Il documento è l'unità centrale del sistema.

Non il progetto.

Non il workspace.

Non il database.

Non la sessione.

Tutte le funzionalità devono partire dal documento.

## Window Philosophy

Una finestra rappresenta un contesto di scrittura.

L'utente può aprire più finestre indipendenti.

Esempio:

Finestra 1:
Documento A

Finestra 2:
Documento B

Finestra 3:
Documento C

Le finestre possono essere affiancate dal window manager.

## Interface Philosophy

Interfaccia classica desktop.

Componenti fondamentali:

- Menu Bar
- Editor
- Status Bar

Elementi da evitare:

- Ribbon
- Dashboard
- Sidebar permanenti
- Workspace complexity
- IDE layouts

## File Philosophy

Formati principali:

- TXT
- MD

Encoding principale:

- UTF-8

Il file sul disco rimane la fonte di verità.

## Writing First

Ogni decisione progettuale deve rispondere a una domanda:

"Questo migliora realmente la scrittura?"

Se la risposta è no, la feature non appartiene a Stilus.

