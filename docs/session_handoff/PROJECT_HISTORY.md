# STILUS PROJECT HISTORY

Purpose:

This document preserves the high-level engineering history of Stilus.

It is intended for future development sessions, audits, onboarding, and project continuity.

---

## Phase 0

### AirPad Import

Baseline imported from AirPad.

Commit:

stilus-import-baseline

Purpose:

Establish a stable upstream baseline before any modification.

---

### Build Audit

Milestone:

P0-H1

Goal:

Verify that the imported project builds correctly.

Result:

PASS

Outcome:

Executable successfully produced.

Build process documented.

---

### Save Lifecycle Audit

Milestone:

P0-H2

Goal:

Audit document save lifecycle.

Findings:

P0-001 Empty Buffer Save Unsafe Read

P0-002 Save As NULL Safety

P0-003 Save Logic Duplication

Result:

PASS

Outcome:

Issues documented and preserved.

---

### Upstream Hardening Library

Goal:

Preserve all inherited defects and fixes.

Created:

docs/upstream_hardening/

Policy:

docs/canonical/UPSTREAM_HARDENING_POLICY.md

Purpose:

Engineering traceability.

Regression prevention.

Intellectual honesty.

---

### UTF-8 Save As Failure

Identifier:

P0-004

Discovery Method:

Real desktop validation.

Observed Behavior:

ASCII text:

PASS

UTF-8 accented text:

FAIL

Examples:

PASS

* test
* questo e un test

FAIL

* è
* àèìòù

Error:

Invalid byte sequence in conversion input

Result:

Root cause identified.

Patch prepared.

---

### UTF-8 Save As Hardening

Result:

Desktop Validation PASS

Verified:

* test
* è
* àèìòù
* questo è un test di stilus

Conclusion:

UTF-8 Save As path corrected.

---

### UTF-8 Open Failure

Identifier:

P0-004B

Discovery Method:

Desktop validation after Save As fix.

Observed Behavior:

Save As:

PASS

Open/Reopen:

FAIL

Error:

Converting contents of file failed:

Invalid byte sequence in conversion input

Conclusion:

Write path appears fixed.

Read/Open conversion path remains defective.

Status:

OPEN

Current Mandatory Order:

P0-004B UTF-8 OPEN FAILURE

---

## Current Project State

Phase:

0 Hardening

Active Blocker:

P0-004B UTF-8 OPEN FAILURE

Rule:

No feature development until active blocker is closed.

---

## Planned Future Milestones

After P0-004B:

P0-H3 Undo/Redo Audit

P0-H4 Memory Ownership Audit

P0-H5 Desktop Stress Audit

---

End of history.

---

### UTF-8 Open Failure Resolution

Milestone:

P0-004B

Status:

RESOLVED

Origin:

Inherited AirPad defect.

Problem:

UTF-8 files containing accented characters could be saved successfully but could not be reopened successfully.

Observed error:

Invalid byte sequence in conversion input

Root Cause:

AirPad derived the default file encoding from the current locale using:

    g_get_codeset()

This created inconsistent save/open behavior on systems not defaulting to UTF-8.

Historical Model:

Modern writing editors treat UTF-8 as the default text encoding.

Legacy encodings are compatibility paths, not defaults.

Resolution:

Replaced locale-derived default encoding with:

    g_strdup("UTF-8")

Commit:

cfe347b

Validation:

- empty file: PASS
- ASCII file: PASS
- UTF-8 accented file: PASS
- mixed ASCII/UTF-8 file: PASS

Result:

UTF-8 Open Failure resolved.

