# STILUS BACKUP AND RECOVERY SPEC

Status: CANONICAL

## Purpose

Stilus must protect long-form writing against:

- accidental closure;
- application crash;
- system crash;
- power loss;
- save failures;
- user mistakes.

Recovery is a core feature.

---

## Design Principles

Writing safety before convenience.

Data preservation before cleanup.

Simple and predictable behavior.

No hidden destructive actions.

---

## Recovery Levels

### Level 0

Normal save.

User saves document.

No recovery action required.

---

### Level 1

Backup copy.

Every successful save may maintain a backup copy.

Purpose:

- protect against accidental overwrite;
- protect against save corruption.

---

### Level 2

Crash recovery snapshot.

Unsaved work must be recoverable.

Recovery data is temporary.

Recovery data is never considered authoritative.

The original document remains authoritative.

---

### Level 3

Emergency save.

If a normal save fails:

- preserve user text;
- create emergency recovery file;
- notify user.

Data loss must be avoided whenever possible.

---

## Recovery Startup

At startup Stilus shall check:

- recovery files;
- recovery snapshots;
- interrupted sessions.

If recoverable content exists:

Restore

Discard

must be offered.

---

## External Modification

Future integration:

P3-R-002

If a file changes outside Stilus:

- detect change;
- notify user;
- offer reload.

Silent overwrite is forbidden.

---

## Session Restore

Future integration:

P3-R-004

May restore:

- open document;
- cursor position;
- window state.

Session restore is independent from crash recovery.

---

## Explicit Non-Goals

Version control.

Document history browser.

Cloud synchronization.

Collaborative editing.

---

## Success Criteria

Unexpected termination shall not normally cause loss of significant writing work.

