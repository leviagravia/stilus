# STILUS AUDIT FINDINGS REGISTER

Status: CANONICAL

## Purpose

The Audit Findings Register is the canonical repository of evidence-based technical findings discovered during certified Stilus audits.

The register exists to preserve technical knowledge independently from:

- implementation decisions;
- known issues;
- roadmap priorities;
- future Mandatory Orders.

The register does not authorize code changes.

The register does not authorize implementation work.

The register records evidence only.

---

## Classification Rules

Each finding receives a unique identifier:

AF-001
AF-002
AF-003
...

Each finding must originate from a completed audit.

Each finding must be supported by repository evidence.

---

## Finding Template

### AF-XXX — Title

Status:
OPEN | CLOSED | SUPERSEDED

Area:

Source Audit:

Evidence:

Repository Location:

Risk:

Current Decision:

Future Investigation:

Notes:

---

## AF-001 — GTK Deprecated APIs remain in active use

Status:
OPEN

Area:
GTK / Technical Debt

Source Audit:
P2-004 GTK Deprecation Cleanup Audit

### Evidence

Current HEAD:

14bbccb63c8878c79cf689283e9243a8f7f3eb5f

Current source contains exactly two deprecated GTK API usages:

src/dialog.c:89

gtk_message_dialog_set_image()

src/window.c:443

gtk_widget_override_font()

Current build output reports both APIs as deprecated.

### Repository Location

src/dialog.c:89

src/window.c:443

### Risk

Mixed

gtk_message_dialog_set_image():
NEEDS MORE EVIDENCE

gtk_widget_override_font():
HIGH RISK

### Current Decision

No implementation authorized.

GTK Deprecation Cleanup remains audited only.

### Future Investigation

Dedicated Mandatory Orders are required before any cleanup implementation.

### Notes

The finding records evidence only.

It does not authorize source modifications.
