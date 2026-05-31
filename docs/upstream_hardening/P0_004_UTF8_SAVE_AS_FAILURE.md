# P0-004 UTF-8 Save As Failure

Origin:
AirPad baseline.

Discovery:
User desktop validation.

Reproduction:

PASS:
- test
- questo e un test

FAIL:
- è
- àèìòù

Observed Error:

Invalid byte sequence in conversion input.

Finding:

ASCII text saves correctly.

UTF-8 accented text fails during Save As.

Severity:
P0

Status:
Under investigation.

Importance:

This was the first defect discovered through real desktop validation rather than static audit.
