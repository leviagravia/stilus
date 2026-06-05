# KNOWN ISSUES

Status: CANONICAL

This document tracks known defects, caveats, deferred fixes and accepted temporary limitations in Stilus.

---

## KI-001 — GtkAboutDialog follows system locale

Status: RESOLVED

Severity: LOW

Area: UI / Localization / About

### Problem

The main Stilus UI is currently English by default, but the About dialog may appear in the system language.

Example on Italian desktop sessions:

- Informazioni su Stilus
- Sito web
- Riconoscimenti
- Chiudi

### Cause

The previous About dialog used GTK's built-in `GtkAboutDialog`.

GTK localizes internal dialog labels according to the desktop/system locale.

This is independent from Stilus application strings.

### Impact

No data loss.

No editing failure.

No file lifecycle risk.

User interface language consistency is affected.

### Resolution

`GtkAboutDialog` was replaced by the custom Stilus Help Layer.

About Stilus now uses Stilus-owned GTK dialogs and English strings.

### Verification

PASS

Validated through Help → About Stilus after `stilus-help-layer-certified`.

### Remaining Caveat

Other GTK-native dialogs may still follow the desktop/system locale. This is outside the resolved About Layer issue.


---

## KI-002 — Application icon is a PNG embedded inside an SVG container

Status: RESOLVED

Severity: LOW

Area: Branding / Packaging / Icons

### Problem

The current Stilus application icon is distributed as an SVG file containing an embedded PNG image.

As a consequence:

- the icon is larger than necessary;
- it is not a true vector icon;
- scaling quality depends on the embedded raster image;
- package size is slightly increased.

### Cause

The current branding phase prioritized functional rebranding over icon optimization.

### Impact

No data loss.

No editing failure.

No runtime instability.

Only affects packaging quality and icon scalability.

### Resolution

The icon was replaced with a true SVG as part of the certified Help Layer work.

Validation result:

- no `<image>`;
- no `base64`;
- no `image/png`;
- no `image/jpeg`.

### Verification

PASS

Validated before `stilus-help-layer-certified`.


---

## KI-003 — Live line/column update via mark-set caused instability

Status: RESOLVED

Severity: MEDIUM

Area: Status Bar / Cursor Position / GTK Signals

### Original Problem

An initial attempt to update `Ln / Col` live in the status bar caused Stilus to disappear during editor interaction, including copy/paste testing.

### Root Cause

The implementation connected `airpad_window_update_status_bar()` directly to the GtkTextBuffer `mark-set` signal.

The callback signature was not compatible with the GTK signal.

### Resolution

A dedicated wrapper callback was implemented for `mark-set`.

The wrapper validates the signal context and safely invokes the central status bar update function.

### Verification

PASS

Validated through:

- cursor movement;
- keyboard navigation;
- mouse navigation;
- copy/paste operations;
- live word count updates;
- live text size updates.

No instability observed.

### Result

`Ln` and `Col` are now updated live in the status bar.


---

## KI-004 — About dialog may show fallback icon instead of Stilus icon

Status: RESOLVED

Severity: LOW

Area: UI / Branding / Icons / About

### Problem

The About dialog may show a small fallback/default icon instead of the Stilus application icon.

### Cause

The previous About implementation loaded the icon from the active GTK icon theme using `AIRPAD_EXEC_NAME`.

If the Stilus icon is not installed in the current icon theme path, GTK falls back to a default icon.

### Impact

No data loss.

No editing failure.

No runtime instability.

Branding consistency is affected.

### Resolution

The custom Stilus Help Layer no longer uses `GtkAboutDialog` for About Stilus.

The About icon is loaded from a project-controlled source and no longer depends on GTK About icon-theme fallback behavior.

### Verification

PASS

Validated through the certified Help Layer implementation.


---

## KI-005 — Undo/Redo relies on menu state validation

Status: ACCEPTED

Severity: LOW

Area: Undo / Redo / Defensive Programming

### Problem

`airpad_undo_action_undo()` and `airpad_undo_action_redo()` do not perform explicit local boundary validation.

The implementation assumes:

- Undo is only invoked when undo history exists.
- Redo is only invoked when redo history exists.

### Cause

The current implementation relies on GTK menu sensitivity state.

The Edit menu disables:

- Undo when `undo_buffer.next == 0`
- Redo when `undo_buffer.next == undo_buffer.end`

As a consequence, invalid user invocation is prevented by the UI layer rather than by local guards inside the action functions.

### Desktop Validation

PASS

Validated through:

- new document
- edit
- undo
- redo
- save
- dirty state transitions
- undo/redo boundary transitions

No incorrect behavior observed.

### Impact

No user-visible malfunction observed.

No data loss observed.

No crash observed.

### Current Decision

Accepted.

The current implementation behaves correctly under all validated desktop scenarios.

### Future Hardening

Optional defensive guards may be added to:

- `airpad_undo_action_undo()`
- `airpad_undo_action_redo()`

to tolerate invalid direct invocation paths.

### Certification Impact

None.

The issue does not block Stilus Core Certification.

---

## KI-006 — Remaining GTK-native dialogs may follow system locale

Status: OPEN

Severity: LOW

Area: UI / Localization / GTK dialogs

### Problem

Some GTK-native dialogs outside the custom Help Layer may still expose system-localized labels.

Examples may include file chooser dialogs, font chooser dialogs or stock GTK button labels.

### Cause

These dialogs are provided by GTK and may use the desktop/system locale for their internal labels.

### Impact

No data loss.

No editing failure.

No file lifecycle risk.

User interface language consistency may be affected.

### Current Decision

Accepted temporary limitation.

The custom Help Layer is resolved and uses Stilus-controlled English strings.

Remaining GTK-native dialog localization is a separate issue.

### Future Fix

Evaluate whether each GTK-native dialog should be replaced, wrapped or accepted.

Do not patch this opportunistically.

---

## KI-007 — Deprecated GTK APIs remain in use

Status: OPEN

Severity: LOW

Area: GTK / Technical Debt

### Problem

Build warnings indicate deprecated GTK APIs still in use.

Known examples:

- `gtk_message_dialog_set_image`
- `gtk_widget_override_font`

### Impact

No data loss.

No editing failure.

No observed runtime instability.

### Current Decision

Accepted technical debt.

These APIs should not be changed opportunistically during unrelated work.

### Future Fix

Handle in a dedicated GTK deprecation audit and cleanup phase.

