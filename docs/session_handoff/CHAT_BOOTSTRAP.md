# CHAT BOOTSTRAP

Purpose:

This document is the mandatory starting point for any new ChatGPT session working on Stilus.

Do not rely on chat summaries.

Verify project state directly from the repository.

---

## Repository

Official repository:

Stilus

Branch:

main

---

## Mandatory Startup Procedure

Before providing recommendations, plans, audits, patches, feature proposals, or mandatory orders:

Read:

docs/session_handoff/PHASE0_STATUS.md

Read:

docs/session_handoff/PROJECT_HISTORY.md

Read:

docs/mandatory_orders/MANDATORY_ORDER_METHOD.md

Read:

docs/canonical/UPSTREAM_HARDENING_POLICY.md

Inspect:

docs/upstream_hardening/

Inspect:

docs/audits/

Inspect:

docs/canonical/

---

## Mandatory Terminal Verification

Request the user to execute:

cd ~/Projects/stilus

git status

git log --oneline -10

Then inspect the current repository state before making assumptions.

---

## Rules

Never assume repository state from previous chats.

Never assume current branch.

Never assume current commit.

Never assume active blocker.

Verify first.

---

## Current Known Blocker

P0-004B UTF-8 OPEN FAILURE

Status:

OPEN

Desktop Evidence:

UTF-8 Save As = PASS

UTF-8 Open/Reopen = FAIL

Error:

Invalid byte sequence in conversion input

---

## Engineering Policy

No feature work while active P0 blockers remain open.

Hardening before expansion.

Evidence before conclusions.

Repository before memory.

---

End of bootstrap.
