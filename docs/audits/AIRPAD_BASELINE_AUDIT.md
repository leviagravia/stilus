# AirPad Baseline Audit

## Purpose

This document records the initial audit of the AirPad source before importing it as the Stilus seed core.

AirPad is treated as a seed core to be imported, attributed, hardened, and evolved independently.

## Source

* upstream/airpad-main.zip
* upstream/airpad-main/

## Strengths

* small codebase
* GTK native
* modular source layout
* separate file lifecycle
* separate undo module
* separate search/replace modules
* separate quit module
* understandable architecture

## Known Risks

### Empty Buffer Save

Pattern:

contents[contents_length - 1]

Risk:

* invalid read when contents_length == 0

### Save As From New Document

Pattern:

g_file_equal(current_file, ...)

Risk:

* current_file may be NULL

### Save Logic Duplication

Observed in:

* file.c
* aux.c

Risk:

* inconsistent fixes
* divergent behavior

### Undo / Redo Scroll Behavior

Risk:

* viewport movement during undo/redo

## Phase 0 Hardening Targets

1. Empty-buffer save guard
2. Save As NULL safety
3. Save lifecycle audit
4. Memory ownership audit
5. Undo/redo stress testing
6. Save/Save As stress testing

## Baseline Policy

Import AirPad unchanged.

Create tag:

stilus-import-baseline

All fixes occur after baseline import.
