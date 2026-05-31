# Calamus Lessons Learned

## Purpose

This document records the most important engineering lessons learned during the Calamus development cycle.

Its purpose is preventing Stilus from repeating the same mistakes.

# Lesson 1 — Architecture Is More Important Than Features

Do not add features to compensate for architectural weaknesses.

Fix architecture first.

# Lesson 2 — Undo/Redo Is A Core System

Undo/Redo is infrastructure, not a secondary feature.

# Lesson 3 — User-Visible Behavior Is The Real Specification

Desktop validation remains mandatory.

# Lesson 4 — Fail-First Can Be Faked

A failing test is valid only if it reproduces the original bug.

# Lesson 5 — Model Tests Are Not Application Tests

Real application paths matter.

# Lesson 6 — Historical Software Matters

Historical software is a library of solved problems.

# Lesson 7 — Mandatory Orders Work

Mandatory Orders dramatically improve reliability.

# Lesson 8 — Desktop Validation Cannot Be Automated Away

Cursor, scrolling, focus and selection require real validation.

# Lesson 9 — Root Cause Before Patch

Problem
→ Root Cause
→ Model
→ Validation
→ Patch

Never:

Problem
→ Patch
→ Hope

# Lesson 10 — Knowledge Is A Project Asset

Protect it.
Organize it.
Version it.
Reuse it.
