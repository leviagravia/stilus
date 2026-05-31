# Stilus Architecture

Stilus is organized around two layers:

## Core Editor

The core owns:

- document buffer
- file lifecycle
- dirty state
- undo / redo
- search / replace
- close flow
- preferences

## Editorial Layer

The editorial layer may add:

- typography tools
- statistics
- templates
- clips
- bookmarks
- references
- writing utilities

The editorial layer must not corrupt or bypass the core editor lifecycle.
