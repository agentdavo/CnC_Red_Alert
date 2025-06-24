# AGENTS Instructions

This repository contains the original sources for **Command & Conquer: Red Alert**. The long-term goal is to port the code to standard C11 and remove its reliance on outdated Windows-only libraries.

## Scope

These instructions apply to all files within this repository.

## Porting Guidelines

- Prefer C11-compliant code. Avoid C++ features unless absolutely necessary.
- Replace legacy Windows APIs (e.g., DirectX, HMI SOS) with portable libraries or stub implementations.
- Keep assembly modules intact until a C equivalent is available.
- Document notable changes and dependency replacements in `progress.md`.
- After each modification, run available build or test scripts where possible.

Use this file as a quick reference when planning code conversions or dependency replacements.

## Reference Documentation

Several Markdown files at the repository root contain critical details about the original engine:

- `info.md` – overview of tools and library directories.
- `drawing.md` – explanation of the rendering pipeline and assembly helpers.
- `sound.md` – breakdown of the audio subsystem and DirectSound routines.
- `ddraw.md` – lists DirectDraw usage across the code base.
- `modex.md` – describes the `ModeX_Blit` VGA routine.
- `shadowx.md` – explains the `Shadow_Blit` path for DOS builds.
- `lvgl.md` – documents the LVGL integration. See the `lvgl_init` and `lvgl_blit`
  routines used when `USE_LVGL` is enabled.
- `keyboard.md` – notes on the keyboard handler and LVGL keyboard integration.
- `mouse.md` – notes on the mouse system and LVGL input devices.

Consult these documents when porting or refactoring code.
