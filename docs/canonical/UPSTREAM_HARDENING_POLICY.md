# Upstream Hardening Policy

## Purpose

Stilus originates from imported upstream code.

Imported code may contain defects, design weaknesses, unsafe assumptions, or undocumented behavior.

Every significant inherited defect discovered and corrected by Stilus must be permanently documented.

## Mandatory Rule

A bug fix without historical documentation is considered incomplete.

## Required Sections

Each hardening record must contain:

- Origin
- Discovery
- Reproduction
- Root Cause
- Fix
- Validation
- Remaining Risks

## Goals

- intellectual honesty
- engineering traceability
- regression prevention
- preservation of institutional knowledge

## Scope

This policy applies to:

- AirPad-derived code
- imported external code
- future upstream imports

## Final Rule

Stilus must preserve both successful decisions and discovered failures.

Engineering history is part of the project.
