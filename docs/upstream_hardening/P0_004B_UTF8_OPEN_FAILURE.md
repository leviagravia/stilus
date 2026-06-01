# P0-004B UTF-8 OPEN FAILURE

Status:

RESOLVED

Commit:

cfe347b

Origin:

Inherited from AirPad.

---

## Problem

Files containing UTF-8 accented characters could be:

- saved successfully

but could not be:

- reopened successfully

Observed error:

Invalid byte sequence in conversion input

---

## Root Cause

AirPad initialized the default file encoding using:

    data_arguments->file_encoding = g_get_codeset();

This tied file opening behavior to the current process locale.

On systems where the locale codeset was not UTF-8, files saved in UTF-8 could later be reopened using an incompatible source encoding assumption.

---

## Fix

Replaced:

    g_get_codeset()

with:

    g_strdup("UTF-8")

for the default file encoding.

Users may still explicitly select alternative encodings when required.

---

## Validation

Desktop validation:

PASS

Tests:

- empty file
- ASCII file
- UTF-8 accented file
- mixed ASCII/UTF-8 file

All PASS.

---

## Lessons Learned

The historical editor audits were correct:

UTF-8 should be the default encoding.

Legacy encodings should be opt-in compatibility paths.

Locale-derived default encodings are fragile and can create inconsistent save/open behavior.

---

## Classification

Upstream hardening.

Inherited AirPad defect.

Resolved in Stilus.
