# Anti-Theater 8 Gates

These gates are mandatory for complex Stilus development tasks.

They exist to prevent fake implementation, mock-only validation, generic reports, and non-probative patches.

## 1. Fail-First Validity Proof

A failing test is valid only if it fails for the original user-visible bug.

Invalid failures include:

- API mismatch
- TypeError
- missing method
- syntax error
- test bug

## 2. Real Bug Path Proof

A test must prove which real application path it exercises.

Required:

- entry point
- call chain
- terminal function
- user action
- GTK/runtime involvement if relevant

## 3. Reference Implementation Proof

Historical software references must include:

- file
- function
- struct or data structure
- field or state
- restore/apply order

Generic claims are invalid.

## 4. Data Flow Blueprint

Every patch plan must describe:

- current data flow
- proposed data flow
- event
- function chain
- state mutation
- visible result

## 5. Offset Justification

For cursor/editor bugs, expected offsets must be numeric and justified.

"Near the cursor" is not enough.

## 6. GTK-Removal Validity Check

For GTK/editor lifecycle bugs, a test that would still pass without GTK is not final proof.

## 7. Adversarial Self-Review

The developer must answer:

- how could this report be faked?
- what prevents that?
- what remains vulnerable?

## 8. Reviewer Checklist

Every report must end with:

- can implementation start?
- missing evidence
- remaining risks
- approved files
- forbidden files
- validation level
