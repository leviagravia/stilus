# L3afpad

## Role in Stilus

L3afpad is a reference implementation and benchmark for classical GTK editor behavior.

It should not be treated as the primary Stilus base unless AirPad proves unsuitable.

## Strengths

- mature lightweight editor lineage
- close to Leafpad philosophy
- useful undo/redo model
- useful file lifecycle behavior
- richer than AirPad in some classical features

## Weaknesses

- older codebase
- more legacy structure
- stronger coupling through callbacks and menu/window logic
- less attractive as a modular foundation

## Relevant Models

- operation-based undo/redo
- classical file lifecycle
- close flow
- DnD
- printing
- encoding handling

## Things Not To Copy Blindly

- legacy architecture
- global state patterns
- callback-heavy structure
- feature accretion without modular boundaries

## Stilus Policy

Use L3afpad as a source of historical solutions and behavioral benchmarks.
