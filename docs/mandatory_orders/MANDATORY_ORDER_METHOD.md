# MANDATORY ORDER METHOD

Purpose:

This document defines the canonical structure of Mandatory Orders used by the Stilus project.

Mandatory Orders are structured engineering directives.

They are used for:

* audits
* hardening
* root cause analysis
* regression investigations
* architecture validation
* implementation validation
* bug fixing

They are not free-form requests.

---

## Core Principle

Adapt the order to the task.

Keep the structure stable.

Do not invent a new format for every issue.

---

## Canonical Sections

A Mandatory Order should normally contain:

1. Title

2. Mission / Objective

3. Context

4. Authorized Modification Files

5. Read-Only Evidence Allowed

6. Modification Forbidden

7. Mandatory Investigation

8. Mandatory Search

9. Required Evidence

10. Required Deliverables

11. Rejection Conditions

12. Acceptance Conditions

13. Final Verdict

---

## Read-Only vs Modification

These concepts are different.

Read-Only Evidence Allowed:

Files may be inspected, searched, quoted and used as evidence.

Modification Forbidden:

Files may not be modified.

A file may belong to both categories.

Example:

window.c

may be read

may not be modified

---

## Evidence First

Mandatory Orders require evidence.

Examples:

* code evidence
* call-chain evidence
* lifecycle evidence
* ownership evidence
* desktop validation evidence
* regression evidence

Assertions without evidence are insufficient.

---

## Anti-Theater Requirement

All Mandatory Orders must comply with:

anti_theater_8_gates.md

Evidence takes precedence over narrative.

---

## Project Context

Mandatory Orders operate within the constraints defined by:

* ENGINEERING_METHOD.md
* UPSTREAM_HARDENING_POLICY.md
* PROJECT_MILESTONES.md

and all canonical project documents.

---

## Examples

Examples include:

* Build Audit
* Save Lifecycle Audit
* UTF-8 Save As Investigation
* UTF-8 Open Hardening Validation
* Root Cause Analysis
* Regression Analysis
* Architecture Validation
* Hardening Verification

End of document.
