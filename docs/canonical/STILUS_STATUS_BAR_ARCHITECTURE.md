# STILUS STATUS BAR ARCHITECTURE

Status: CANONICAL

## Purpose

Define how the Stilus status bar receives document state without creating unsafe dependencies.

---

## Current Dynamic Fields

Implemented:

- Saved / Unsaved
- Word count

Static placeholders:

- Writable / Protected
- Language
- Line
- Column
- Encoding
- File type
- File size

---

## Current Limitation

The current status bar update function receives:

- GtkTextBuffer
- AirpadDataWindow

It does not receive:

- AirpadDataFile
- AirpadDataApplication

Therefore file-dependent fields such as file size and file type must not be implemented by guessing or adding hidden global state.

---

## Required Direction

Future status bar updates should be driven by a dedicated status context or application-level update function that can safely access:

- text buffer
- window widgets
- current file
- encoding
- document metadata

---

## Non-Goals

No global mutable status state.

No file metadata lookup from UI-only functions.

No unsafe callback signature reuse.

---

## Known Related Issue

Live line/column updates require a dedicated signal wrapper.

Do not connect incompatible callbacks directly to GTK signals.

