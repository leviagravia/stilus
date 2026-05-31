# AirPad

## Role in Stilus

AirPad is the primary seed-core candidate for Stilus.

It is valuable because it is small, GTK-based, relatively modular, and focused on classical text editing.

## Strengths

- small codebase
- GTK3 foundation
- modular source layout
- separate files for file, undo, search, replace, quit, view
- good candidate for a maintainable core

## Weaknesses

- project appears inactive
- missing classical features such as printing
- Save As semantics required hardening
- duplicated write logic must be monitored
- undo/redo scroll behavior still requires validation

## Relevant Models

- modular file organization
- operation-based undo/redo
- separated search and replace modules
- explicit quit/close handling

## Things Not To Copy Blindly

- duplicated save/write logic
- unsafe boundary assumptions
- any unvalidated Save As behavior
- aggressive scroll behavior during undo/redo

## Stilus Policy

AirPad may provide the initial seed core.

It must be hardenized before Stilus features are added.
