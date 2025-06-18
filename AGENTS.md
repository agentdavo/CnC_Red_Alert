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

## Reference Documentation

Several Markdown files at the repository root contain critical details about the original engine:

- `INFO.md` – overview of tools and library directories.
- `DRAWING.md` – explanation of the rendering pipeline and assembly helpers.
- `SOUND.md` – breakdown of the audio subsystem and DirectSound routines.
- `DDRAW.md` – lists DirectDraw usage across the code base.
- `MODEX.md` – describes the `ModeX_Blit` VGA routine.
- `SHADOWX.md` – explains the `Shadow_Blit` path for DOS builds.
- `LVGL.md` – documents the LVGL integration. See the `lvgl_init` and `lvgl_blit`
  routines used when `USE_LVGL` is enabled.
- `KEYBOARD.md` – notes on the keyboard handler and LVGL keyboard integration.
- `MOUSE.md` – notes on the mouse system and LVGL input devices.

Consult these documents when porting or refactoring code.
