# STILUS HELP LAYER SPEC

Status: CANONICAL

## Purpose

The Help Layer defines the user-facing informational dialogs of Stilus.

It replaces the legacy single GTK About dependency with a Stilus-controlled help structure.

The Help Layer exists to communicate:

- application identity;
- project attribution;
- writing principles;
- keyboard shortcuts;
- writing workflow.

---

## Help Menu Structure

Help

- About Stilus
- Writing Principles
- Keyboard Shortcuts
- Writing Workflow

---

## About Stilus

### Responsibility

About Stilus communicates the identity of the application.

It must remain compact.

It must not become a documentation container.

### Required Content

Stilus

Version 0.1.x

Created by Luciano Squizzato

A lightweight text editor designed for writers

Website

GNU General Public License v3 or later

Credits

### Credits Access

About Stilus must contain a Credits button.

Credits must open a separate Credits dialog.

---

## Credits

### Responsibility

Credits communicates authorship, icon attribution, origin and license preservation.

### Required Content

Created by Luciano Squizzato

Stilus icon by Luciano Squizzato

Stilus originates from the AirPad project by Nikola Hadžić.

The Stilus project preserves attribution and licensing information in accordance with the GNU General Public License.

---

## Writing Principles

### Responsibility

Writing Principles communicates the editorial philosophy of Stilus.

### Source

docs/philosophy/WRITING_PRINCIPLES.md

### Required Behavior

Writing Principles must be separate from About Stilus.

It must not be embedded inside About Stilus.

---

## Keyboard Shortcuts

### Responsibility

Keyboard Shortcuts communicates operational help.

### Source

docs/help/KEYBOARD_SHORTCUTS.md

### Required Behavior

Keyboard Shortcuts must be separate from About Stilus.

It must not be embedded inside About Stilus.

---

## Writing Workflow

### Responsibility

Writing Workflow communicates the intended writing process supported by Stilus.

### Source

docs/help/WRITING_WORKFLOW.md

### Required Behavior

Writing Workflow must be separate from About Stilus.

It must not be embedded inside About Stilus.

---

## Language Policy

The Help Layer must use Stilus-controlled English strings by default.

It must not depend on GTK stock About dialog localization.

GtkAboutDialog must not be used for the final Help Layer implementation.

---

## Icon Policy

The Help Layer must not load the application icon from the active GTK icon theme for About display.

The About icon must be loaded from a project-controlled source.

The official Stilus icon must be a true SVG.

Forbidden:

- embedded PNG inside SVG;
- embedded JPEG inside SVG;
- base64 raster image inside SVG.

Required validation:

- no `<image>` tag;
- no `base64`;
- no `image/png`;
- no `image/jpeg`.

---

## Implementation Policy

The Help Layer must be implemented as Stilus-controlled GTK dialogs.

Allowed:

- GtkDialog
- GtkBox
- GtkLabel
- GtkButton
- GtkImage
- GtkScrolledWindow where needed

Forbidden for final implementation:

- GtkAboutDialog
- GTK stock About layout
- GTK translator credits
- icon-theme-dependent About logo loading

---

## Architectural Rule

Each Help Layer dialog must have one responsibility.

About Stilus must not contain:

- full Writing Principles;
- full Keyboard Shortcuts;
- full Writing Workflow;
- long project history.

Credits must not contain operational help.

Keyboard Shortcuts must not contain philosophy.

Writing Workflow must not contain credits.

---

## Acceptance Conditions

The Help Layer is accepted only when:

- About Stilus opens from Help → About Stilus;
- Credits opens from the About Stilus Credits button;
- Writing Principles opens separately;
- Keyboard Shortcuts opens separately;
- Writing Workflow opens separately;
- the About icon is the Stilus icon;
- About does not show the GTK fallback icon;
- visible Help Layer labels are English by default;
- GtkAboutDialog is no longer used for About Stilus;
- source build passes;
- desktop validation passes.

