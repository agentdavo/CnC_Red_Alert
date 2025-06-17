# AGENTS Instructions

This repository contains the original sources for **Command & Conquer: Red Alert**. The long-term goal is to port the code to standard C11 and remove its reliance on outdated Windows-only libraries.

## Scope

These instructions apply to all files within this repository.

## Porting Guidelines

- Prefer C11-compliant code. Avoid C++ features unless absolutely necessary.
- Replace legacy Windows APIs (e.g., DirectX, HMI SOS) with portable libraries or stub implementations.
- Keep assembly modules intact until a C equivalent is available.
- Document notable changes and dependency replacements in `PROGRESS.md`.
- After each modification, run available build or test scripts where possible.

Use this file as a quick reference when planning code conversions or dependency replacements.
