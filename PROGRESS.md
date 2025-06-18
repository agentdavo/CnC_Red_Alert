# Porting Progress

This document tracks the ongoing effort to port **Command & Conquer: Red Alert** to a modern C11 code base.

## External dependencies

The original source relies on several legacy libraries that are no longer readily available. Each of these will need a modern replacement or a temporary stub in order to build the project:

- **DirectX 5 SDK** – handles graphics and input. Replace with a cross-platform rendering library (e.g., SDL, OpenGL) or provide stubs for testing.
- **DirectX Media 5.1 SDK** – used for video playback. Provide an alternative video subsystem or stub implementation.
- **Greenleaf Communications Library (GCL)** – provides networking/communication routines. Substitute with standard networking APIs or remove if unused.
- **Human Machine Interface (HMI) "Sound Operating System" (SOS)** – provides audio support. Replace with a modern audio library or stub.
- **IPX networking components** – some modules expect IPX APIs. Implement wrappers around modern sockets or stub out for now.

As the port progresses, updates on how each dependency has been replaced or stubbed should be recorded here.
- Converted LAUNCH assembly launcher to portable C11 (launch/main.c).
- Replaced legacy bool typedef with <stdbool.h> (function.h).
- Replaced HMI SOS timer callbacks with a miniaudio-based implementation (src/miniaudio.c).
- Added the official miniaudio library under `src/miniaudio` and integrated it via CMake.
- Replaced bit manipulation assembly pragmas in jshell.h with portable C.
- Removed references to legacy Borland/Watcom tooling in `CODE/MAKEFILE` in favor of the CMake build system.
- Removed DOS/4GW stub launcher (CWSTUB.C) and launch/main.c now runs the game directly.
- Replaced bit manipulation assembly pragmas in jshell.h with portable C.
- Removed segmentation keywords (far/near/huge) from legacy headers for C11 compliance.
- Launcher now relies only on standard C headers; disk and swap file handling use stub implementations.
- Renamed files in LAUNCH and LAUNCHER directories to lowercase for cross-platform compatibility.
- Identified program entry points: `Start` in `LAUNCH/launch.asm` (ported as `launch_main`) and `WinMain` in `CODE/STARTUP.CPP`.
- Added debug logging macros for tracing execution (src/debug_log.h).
- Legacy `WWDebugString` calls now map to `LOG_CALL` via `LOG_MSG` for
  consistent formatting.
- Cleaned up function.h includes for C11 compatibility and wrapped Windows-only headers.
- Documented legacy pragmas and removed `watcom.h` includes (docs/pragma.md).
- Added stub `CODE/wwlib32/wwlib32.h` for missing Windows library.
- Fixed mismatched preprocessor directives in `ftimer.h` and `rect.h`.
- Documented legacy pragmas and removed `watcom.h` includes (PRAGMA.md).
- Added stub `CODE/wwlib32/wwlib32.h` for missing Windows library.
- Fixed mismatched preprocessor directives in `ftimer.h` and `rect.h`.
- Added an LVGL bridge module (`CODE/lvgl/lvgl_bridge.c`) for converting 8-bit screens to an LVGL canvas. Use `USE_LVGL` to enable the call from `GScreenClass::Blit_Display`.
- Created a minimal DirectDraw shim (`src/ddraw`) so the code can build without legacy headers.
- Documented graphics blit routines in MODEX.md and SHADOWX.md
- Added debug logging macros for tracing execution (src/debug_log.h).
- `GScreenClass::Blit_Display` now bypasses DirectDraw when `USE_LVGL` is
  enabled and hands the frame buffer to `lvgl_blit`.
- When `USE_LVGL` is passed to CMake the LVGL library is initialized at startup
  and `lvgl_blit` copies each frame to a canvas instead of relying on DirectDraw.
- Added a Linux-compatible `conio.h` replacement under `src` and updated the
  build system to search this directory so legacy `<conio.h>` includes resolve.
- `src/debug_log.h` now maps logging calls to LVGL's log module for consistent
  output when `ENABLE_LOGGING` is defined. A minimal `lv_conf.h` enables logging
  with `LV_LOG_LEVEL_INFO`.
- Simplified function.h by removing Windows-only assert hooks and redundant includes.
- Implemented an LVGL-based terminal widget for on-screen debugging (`terminal.c` and `terminal.h`).
- Removed stray `#endif` lines in several headers to fix build errors.
