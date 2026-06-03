# STILUS SESSION RECOVERY ARCHITECTURE

Status: CANONICAL

## Purpose

Protect writing work against unexpected application termination.

The primary goal is preservation of user text.

---

## Scope

This architecture applies to:

- application crashes;
- operating system crashes;
- forced process termination;
- unexpected power loss.

---

## Recovery Principles

Recovery must prioritize:

1. Data preservation
2. User transparency
3. Simplicity

Recovery shall never silently discard recoverable text.

---

## Recovery Layers

### Layer 1

Normal Save

User saves document.

---

### Layer 2

Backup on Save

The previous on-disk version is preserved.

Current implementation:

filename.ext.bak

---

### Layer 3

Recovery Snapshot

Unsaved text may be preserved independently from the original document.

Snapshots are recovery artifacts.

They are not normal user files.

---

### Layer 4

Recovery Prompt

On startup Stilus shall detect recovery artifacts.

If recovery data exists the user shall be notified.

The user chooses:

- Recover
- Discard

---

## Initial Constraints

Recovery shall not depend on:

- cloud services;
- network access;
- external databases.

Recovery must function offline.

---

## Non-Goals

Version control.

Document history.

Collaborative editing.

---

## Design Principles

Writing first.

Never silently discard recoverable text.

Predictable behavior.

Low complexity.

