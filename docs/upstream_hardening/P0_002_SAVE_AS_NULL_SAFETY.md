# P0-002 Save As NULL Safety

Origin:
AirPad baseline.

Discovery:
P0-H2 Save Lifecycle Audit.

Affected File:
- src/file.c

Unsafe Pattern:

g_file_equal(current_file, ...)

Failure Mode:

Potential NULL argument during Save As on new unsaved document.

Severity:
P0

Status:
Fixed in Stilus hardening phase.
