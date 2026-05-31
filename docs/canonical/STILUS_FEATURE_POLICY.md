# Stilus Feature Policy

## Purpose

This document defines which features may enter Stilus, which features are forbidden, and which features require architectural review.

Features are not neutral.

Every feature increases maintenance cost.

## Admission Rule

A feature may enter Stilus only if it directly supports:

- writing
- revision
- document navigation
- text cleanup
- text preservation
- file safety
- editorial workflow

If a feature does not support these goals, it does not belong in Stilus.

## Allowed Feature Families

The following feature families are aligned with Stilus:

- typography tools
- statistics
- templates
- bookmarks
- references
- clip collection
- writing utilities
- search and replace
- document navigation
- file lifecycle safety
- preferences
- printing
- recent files
- external file modification detection

## Forbidden Feature Families

The following feature families are forbidden:

- AI writing
- cloud synchronization
- collaboration platform
- plugin system
- Electron application model
- IDE features
- workspace manager
- project manager
- terminal integration
- language server integration
- database-backed knowledge base
- web browser integration

## Features Requiring Architectural Review

The following features are not automatically forbidden, but require explicit architectural review:

- tabs
- split view
- session restore
- markdown preview
- rich text
- outline view
- side panels
- command palette
- macro system
- document metadata

## Import Rule

No feature is imported from Calamus automatically.

Every feature must be classified as:

- Import
- Rewrite
- Reject
- Defer

## Core Protection Rule

The editorial layer must never bypass the editor core.

Features must not corrupt or bypass:

- buffer state
- undo / redo
- dirty state
- save lifecycle
- close flow
- search state

## Final Rule

Stilus must remain a writing editor.

Feature growth must never turn Stilus into an IDE, office suite, cloud tool, or plugin platform.
