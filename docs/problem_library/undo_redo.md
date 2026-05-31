# Undo / Redo

## Status

Known critical editor subsystem.

## Why It Matters

Undo and redo are core editor functions.

Failures in undo/redo damage user trust and can cause data loss, cursor corruption, viewport instability, or inconsistent editor state.

## Historical Findings

Historical editors do not reconstruct cursor state heuristically.

They store explicit operation information and restore state deterministically.

Relevant reference implementations:

- AirPad
- L3afpad
- Leafpad
- Vim

## Known Failure Modes

- cursor restored to wrong offset
- selection restored incorrectly
- viewport scrolling unexpectedly
- redo chain corruption
- modified state inconsistency
- large document slowdown

## Stilus Policy

Undo/redo behavior must be verified through:

- source inspection
- fail-first validation
- regression testing
- desktop validation

No undo/redo implementation is considered stable until desktop validation passes.

## Priority

P0
