# Mandatory Orders

Mandatory Orders govern non-trivial changes in Stilus.

Their purpose is to prevent:

- fake analysis
- fake fail-first validation
- model-only testing
- incomplete root-cause analysis
- scope creep
- architectural drift
- developer theater

Every significant intervention must be bounded by a Mandatory Order.

Mandatory Orders define:

- scope
- evidence requirements
- approved files
- forbidden files
- validation requirements
- artifact requirements
- review criteria

No complex patch should be accepted without a corresponding Mandatory Order.
