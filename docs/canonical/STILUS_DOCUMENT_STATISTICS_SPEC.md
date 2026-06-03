# STILUS DOCUMENT STATISTICS SPEC

Status: CANONICAL

## Purpose

Stilus must provide writing-oriented document statistics without becoming a word processor.

Statistics support long-form writing, academic writing, revision and document awareness.

---

## Status Bar Metrics

The status bar shall expose the most important live metrics:

- Saved / Unsaved
- Writable / Protected
- Language
- Word count
- Line
- Column
- Encoding
- File type
- File size

Canonical example:

Saved | Writable | IT | 12345 words | Ln 321 Col 17 | UTF-8 | MD | 74 KB

---

## Document Statistics Dialog

Future dialog metrics:

- Words
- Characters
- Characters without spaces
- Lines
- Paragraphs
- File size
- Encoding
- File type
- Language

---

## Counting Policy

Words are writing units separated by whitespace.

Characters include all visible text characters.

Characters without spaces exclude whitespace.

Lines follow the GtkTextBuffer line model.

Paragraphs are separated by blank lines.

---

## Initial Implementation Scope

First dynamic implementation:

- Saved / Unsaved
- Word count
- Line
- Column

Later:

- Characters
- Paragraphs
- Reading time
- Language detection

---

## Explicit Non-Goals

Page layout.

Font metrics.

Pagination.

WYSIWYG measurements.

Word processor formatting statistics.

---

## Design Principles

Writing first.

Live feedback.

No visual clutter.

No hidden status indicators.

