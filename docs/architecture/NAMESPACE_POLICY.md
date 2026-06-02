# NAMESPACE POLICY

## Purpose

Stilus originates from AirPad.

To preserve architectural traceability, upstream namespaces are retained until a future controlled migration phase.

---

## Current Rule

### Upstream Code

Prefixes:

- airpad_
- Airpad
- AIRPAD_

Meaning:

Inherited from AirPad upstream.

Examples:

- airpad_file_open()
- airpad_undo_init()
- struct AirpadDataWindow
- AIRPAD_UNDO_BUFFER_ITEM_TYPE_INSERT

---

### Stilus Code

Prefixes:

- stilus_
- Stilus
- STILUS_

Meaning:

Introduced by the Stilus project.

Examples:

- stilus_session_restore()
- stilus_document_statistics()
- struct StilusSession
- STILUS_DEFAULT_LANGUAGE

---

## Migration Policy

Mass renaming is forbidden during core stabilization.

The purpose of preserving upstream namespaces is to maintain historical and architectural traceability.

Future namespace migration, if any, must be performed as a dedicated project phase.

---

## Decision Rule

When adding new functionality:

- Extend existing upstream modules using existing names when appropriate.
- Use Stilus namespaces for new Stilus-specific systems.

---

## Key Question

This policy exists to preserve the ability to answer:

"Was this behavior inherited from AirPad or introduced by Stilus?"
