# STILUS SIMPLEX — Canonical UI Specification

Version: Simplex v1

## UI Philosophy

L'interfaccia deve essere:

- semplice
- classica
- prevedibile
- desktop-native

L'interfaccia non deve ricordare:

- IDE
- browser
- Electron app
- office suite

## Main Window Structure

Top to bottom:

1. Menu Bar
2. Document Area
3. Status Bar

Nient'altro è obbligatorio.

## Menu Bar

Canonical menus:

- File
- Edit
- Search
- Writing
- Notes
- View
- Tools
- Settings
- Help

## File Menu

- New
- Open
- Open Recent
- Save
- Save As
- Close Document
- New Window
- Exit

## Edit Menu

- Undo
- Redo
- Cut
- Copy
- Paste
- Delete
- Select All

## Search Menu

- Find
- Find Next
- Find Previous
- Replace
- Replace All
- Go To Line
- Go To Paragraph

## Writing Menu

- Focus Mode
- Typewriter Mode
- Toggle Right Margin
- Word Count

## Notes Menu

- Insert Footnote
- Next Footnote
- Previous Footnote

## View Menu

- Fullscreen
- Status Bar
- Wrap Text
- Highlight Current Line

## Tools Menu

- Spell Check
- Language

## Settings Menu

- Preferences

## Help Menu

- Keyboard Shortcuts
- About Stilus

## Document Area

Single editor area.

No sidebars required.

No permanent panels.

No project trees.

No workspace views.

## Tabs

Supported.

Each tab contains one document.

Tabs are optional and may be disabled in future editions.

## Multiple Windows

Mandatory.

The user must be able to open:

- Window A
- Window B
- Window C

Each window is independent.

This replaces split views.

## Split Views

Rejected.

Not part of Stilus.

Rejected items:

- Vertical Split
- Horizontal Split
- Pane Manager
- Layout Manager

Reason:

Independent windows are sufficient.

## Status Bar

Mandatory fields:

Saved / Unsaved

Writable / Protected

Language

Word Count

Line

Column

UTF-8

TXT / MD

File Size

## Status Bar Example

Saved | Writable | IT | 12345 words | Ln 321 Col 17 | UTF-8 | MD | 74 KB

## Saved State

Must be explicit.

Allowed:

Saved

Unsaved

Forbidden:

*

•

colored dots

hidden indicators

## Language Indicator

Examples:

IT

EN

FR

DE

ES

## File Protection Indicator

Allowed:

Writable

Protected

Must always be visible.

## File Size

Displayed in:

KB

or

MB

## About Dialog

Must contain:

Stilus

Version

Focused Writing in Plain Text

Author

## Default Visual Style

Neutral.

Calm.

Text-focused.

No decorative UI.

No excessive animations.

## Explicitly Excluded

- Ribbon
- Dashboard
- Workspace Sidebar
- Project Sidebar
- Plugin Sidebar
- AI Chat Panel
- Assistant Panel
- Terminal Panel
- Build Panel
- Debug Panel

