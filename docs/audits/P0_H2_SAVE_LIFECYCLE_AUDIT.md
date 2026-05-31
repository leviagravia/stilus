# P0-H2 Save Lifecycle Audit

## Status

FAIL

## Scope

This audit inspects the AirPad/Stilus baseline save lifecycle before any hardening patch.

Inspected areas:

- src/file.c
- src/aux.c

## Finding 1 — Empty Buffer Save Unsafe Read

### Evidence

src/file.c:96

contents[contents_length - 1]

src/aux.c:124

contents[contents_length - 1]

### Failure Mode

If contents_length == 0, the code reads before the beginning of the buffer.

This is undefined behavior and may cause a crash or invalid memory read.

### Severity

P0

## Finding 2 — Save As NULL File Comparison

### Evidence

src/file.c:357

g_file_equal(current_file, data_application->data_file->file)

### Failure Mode

When Save As is executed from a new unsaved document, current_file may be NULL.

Calling g_file_equal with a NULL argument is unsafe.

### Severity

P0

## Finding 3 — Duplicated Save/Write Logic

### Evidence

Save/write newline logic is present in both:

- src/file.c
- src/aux.c

### Failure Mode

A fix applied to one path may not be applied to the other.

This creates risk of partial hardening and divergent behavior.

### Severity

P1

## Save Topology

airpad_file_write is defined in:

- src/file.c:28

It is called from:

- src/file.c:329
- src/file.c:357

No other direct calls were found in src/*.c.

## Required Hardening

Phase 0 must fix:

1. empty-buffer newline guard in src/file.c
2. empty-buffer newline guard in src/aux.c
3. NULL-safe Save As state machine in src/file.c
4. duplication mapping for file.c / aux.c
5. desktop validation of Save, Save As, Cancel and Failure paths

## Verdict

P0-H2 Save Lifecycle Audit: FAIL

Hardening required before adding any Stilus feature.
