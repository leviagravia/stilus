# Stilus Vision

Stilus is a native desktop writing editor.

Its primary purpose is writing, revision, and long-form text production.

## Core Principles

- Native desktop application
- Lightweight
- Stable
- Maintainable
- Writer-first
- Document-centered

## Stilus Is

- a text editor
- a writing tool
- an editorial environment
- a revision environment

## Stilus Is Not

- an IDE
- a cloud application
- an AI editor
- an office suite
- a plugin platform
- an Electron application

## Architectural Rule

The editor core must remain independent from higher-level writing tools.

Core responsibilities:

- buffer
- file lifecycle
- dirty state
- undo / redo
- search / replace
- close flow
- preferences

Editorial tools must build on top of the core and must never bypass it.

## Long-Term Goal

Create a durable writing editor whose architecture remains understandable and maintainable over time.
