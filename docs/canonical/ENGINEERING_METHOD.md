# Engineering Method

Stilus development is evidence-driven.

The project must not follow the pattern:

bug
patch
bug
patch

Instead, every technical intervention must follow:

problem
evidence
historical model
test
bounded patch
review
desktop validation

## Core Rule

Do not build patches first.

Build understanding first.

The patch must be the consequence of:

- a proven root cause
- a real source path
- a historical implementation model
- a probative test
- a bounded modification contract

## Historical Software

Historical software is not used as code to copy blindly.

Historical software is a library of solved problems.

When Stilus encounters a problem, the required question is:

Who already solved this problem, and what implementation model did they use?

## Mandatory Orders

Complex changes must be governed by Mandatory Orders.

Mandatory Orders exist to prevent:

- fake fail-first tests
- mock-only validation
- generic reports
- unrelated patches
- architectural drift
- developer theater

## Validation Levels

LEVEL 0 — source inspection
LEVEL 1 — unit/model tests
LEVEL 2 — integration tests
LEVEL 3 — automated GTK/runtime validation
LEVEL 4 — real desktop manual validation

No GTK/editor behavior is stable until Level 4 passes.
