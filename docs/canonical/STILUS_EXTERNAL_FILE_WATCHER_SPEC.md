# STILUS EXTERNAL FILE WATCHER SPEC

Status: CANONICAL

## Purpose

Stilus must protect users from silently overwriting files that were modified outside the editor.

---

## Problem

A document may be open in Stilus while another program modifies the same file.

If Stilus saves without noticing the external change, external work may be lost.

---

## Required Behavior

When the current file changes on disk, Stilus shall notify the user.

The user must be offered clear choices:

- Reload
- Keep current document
- Save As

Silent overwrite is forbidden.

---

## Initial Scope

Applies only to files already opened from disk.

Untitled documents are out of scope.

---

## Design Principles

No silent overwrite.

No hidden conflict resolution.

User decides.

Data preservation before convenience.

---

## Future Extensions

Checksum-based validation.

Modification timestamp tracking.

Integration with backup and recovery.

