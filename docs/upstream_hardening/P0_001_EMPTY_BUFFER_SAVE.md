# P0-001 Empty Buffer Save Unsafe Read

Origin:
AirPad baseline.

Discovery:
P0-H2 Save Lifecycle Audit.

Affected Files:
- src/file.c
- src/aux.c

Unsafe Pattern:

contents[contents_length - 1]

Failure Mode:

Possible out-of-bounds read when contents_length == 0.

Severity:
P0

Status:
Fixed in Stilus hardening phase.
