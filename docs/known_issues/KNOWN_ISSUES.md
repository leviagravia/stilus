# KNOWN ISSUES

Status: CANONICAL

This document tracks known defects, caveats, deferred fixes and accepted temporary limitations in Stilus.

---

## KI-001 — GtkAboutDialog follows system locale

Status: OPEN

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

The current About dialog uses GTK's built-in `GtkAboutDialog`.

GTK localizes internal dialog labels according to the desktop/system locale.

This is independent from Stilus application strings.

### Impact

No data loss.

No editing failure.

No file lifecycle risk.

User interface language consistency is affected.

### Current Decision

Do not spend further time patching `GtkAboutDialog`.

Micro-patches to `gtk_window_set_title()` or About dialog properties do not solve the issue.

### Future Fix

Replace `GtkAboutDialog` with a fully custom Stilus About Layer.

The custom About Layer must follow:

- `docs/canonical/STILUS_ABOUT_LAYER_SPEC.md`

### Caveat

Until the custom About Layer is implemented, About may remain localized by GTK even when the Stilus UI is otherwise English.


---

## KI-002 — Application icon is a PNG embedded inside an SVG container

Status: OPEN

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

### Current Decision

Accepted temporary solution.

The current icon remains in use until a proper vector version is produced.

### Future Fix

Create a true vector SVG icon.

Requirements:

- no embedded PNG data;
- fully scalable;
- optimized SVG source;
- maintain current Stilus visual identity.

### Caveat

Current implementation is acceptable for development releases and early project phases.

