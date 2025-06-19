# AGENTS Instructions

This folder contains the original sources for the Window 95 library functions of **Command & Conquer: Red Alert**.

The long-term goal is to port the code to standard C11 and port to alternatives and remove its reliance on ALL Windows-only libraries.

## Scope

These instructions apply to all files within this folder only.

## Porting Guidelines

- Prefer C11-compliant code. Avoid C++ features unless absolutely necessary.
- Replace legacy Windows APIs (e.g., DirectX, HMI SOS) with open source portable libraries or stub implementations.
- Keep assembly modules intact until a C equivalent is available.
- Document notable changes and dependency replacements in `PROGRESS.md`.
- After each modification, run available build or test scripts where possible.

Use this file as a quick reference when planning code conversions or dependency replacements withing this folder.

## Reference Documentation

Several Markdown files in this folder contain critical details about the original library:

- `AGENTS.md` – this document.
- `README.md` – intro.
- `DRAWING.md` – explanation of the rendering pipeline and assembly helpers.
- `SOUND.md` – breakdown of the audio subsystem and DirectSound routines.
- `DDRAW.md` – lists DirectDraw usage across the code base.
- `LVGL.md` – documents the LVGL integration.
- `KEYBOARD.md` – notes on the keyboard handler and LVGL keyboard integration.
- `MOUSE.md` – notes on the mouse system and LVGL input devices.
- `PROGRESS.md` – location where you document work completed per PR.

Consult these documents when porting or refactoring code - update them if anything is wrong or needs clarification. 
