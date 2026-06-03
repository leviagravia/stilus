# STILUS BACKUP FILE POLICY

Status: CANONICAL

## Purpose

Provide a simple and predictable protection layer against accidental overwrite and save corruption.

---

## Backup Strategy

For every successful save of an existing file:

document.txt
→ document.txt.bak

The backup file contains the previously saved version.

---

## Location

Backups are stored in the same directory as the original document.

Example:

novel.txt
novel.txt.bak

---

## Rotation

Only one backup copy is maintained.

Each successful save replaces the previous backup.

---

## Scope

Applies only to files that already exist on disk.

Untitled documents do not generate backups.

---

## Recovery

Users may manually restore:

document.txt.bak
→ document.txt

using standard file operations.

---

## Design Principles

Simple.

Predictable.

Transparent.

No hidden backup locations.

No background backup databases.

---

## Future Extensions

Crash recovery snapshots.

Recovery journal.

Session restore.

These mechanisms are independent from backup files.

