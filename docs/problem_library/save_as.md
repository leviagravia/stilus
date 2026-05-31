# Save As

## Status

Known critical file-lifecycle subsystem.

## Why It Matters

Save As must never corrupt the document path, lose text, clear the modified flag incorrectly, or crash on new unsaved documents.

## Known Failure Modes

- Save As from new document with NULL current file
- failed Save As corrupts current path
- failed Save As clears modified state
- Save As cancel mutates state
- empty-buffer save reads outside buffer
- duplicated write logic creates partial fixes

## Historical Findings

File lifecycle must distinguish:

- success
- failure
- cancel
- new unsaved document
- existing document
- same file
- different file

## Stilus Policy

Save As success accepts the selected path.

Save As failure restores the previous path.

Save As cancel changes nothing.

No Save As patch is accepted without a state-machine table.

## Required Validation

- new empty document Save As
- new non-empty document Save As
- Save As cancel
- Save As failure
- existing document Save As same file
- existing document Save As different file
- failed Save As path preservation
- failed Save As modified-state preservation

## Priority

P0
