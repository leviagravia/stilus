# Vim

## Role in Stilus

Vim is not a Stilus base.

It is a library of mature editor-state models.

## Strengths

- explicit editor state
- mature undo semantics
- cursor and mark handling
- deep navigation model
- long-term stability lessons

## Weaknesses

- modal editor philosophy incompatible with Stilus
- plugin ecosystem not aligned with Stilus
- programmer/editor culture not writer-first
- unsuitable as UI or architectural base

## Relevant Models

- explicit undo cursor state
- marks
- jump history
- deterministic state restoration

## Things Not To Copy Blindly

- modal editing
- plugin system
- scripting layer
- IDE-like expansion

## Stilus Policy

Use Vim only for specific solved editor-state problems.
