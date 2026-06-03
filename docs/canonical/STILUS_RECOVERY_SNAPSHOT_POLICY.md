# STILUS RECOVERY SNAPSHOT POLICY

Status: CANONICAL

## Purpose

Define how Stilus shall preserve unsaved writing work during unexpected termination.

---

## Snapshot Role

Recovery snapshots protect unsaved text.

They are not normal user documents.

They are temporary recovery artifacts.

---

## Storage Location

Snapshots shall be stored under the user data directory.

Canonical target:

~/.local/share/stilus/recovery/

Rationale:

- avoids cluttering document directories;
- keeps recovery artifacts separate from user files;
- follows common Linux desktop conventions.

---

## Snapshot Content

A snapshot should preserve:

- document text;
- original file path, if available;
- timestamp;
- encoding, if known.

---

## Creation Policy

Snapshots may be created when:

- the document is modified;
- unsaved text exists;
- the application is running normally.

Snapshot creation should be periodic or event-driven.

---

## Cleanup Policy

Snapshots shall be removed when:

- the document is saved successfully;
- the document is closed cleanly and the user discards changes;
- the user explicitly discards recovery data.

Snapshots shall not be removed after a crash until the user decides.

---

## Startup Policy

At startup Stilus shall check for recovery snapshots.

If snapshots exist, Stilus shall offer:

- Recover
- Discard

---

## Non-Goals

Full version history.

Multiple timeline snapshots.

Cloud backup.

Collaborative recovery.

---

## Design Principles

Data preservation first.

No silent deletion of recoverable text.

No recovery files mixed with user documents.

