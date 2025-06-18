# Porting Progress

This document tracks the ongoing effort to port **Command & Conquer: Red Alert** to a modern C11 code base.

## External dependencies

The original source relies on several legacy libraries that are no longer readily available. Each of these will need a modern replacement or a temporary stub in order to build the project:

- **DirectX 5 SDK** – handles graphics and input. Replace with a cross-platform rendering library (e.g., SDL, OpenGL) or provide stubs for testing.
- **DirectX Media 5.1 SDK** – used for video playback. Provide an alternative video subsystem or stub implementation.
- **Greenleaf Communications Library (GCL)** – provides networking/communication routines. Substitute with standard networking APIs or remove if unused.
- **Human Machine Interface (HMI) "Sound Operating System" (SOS)** – provides audio support. Replace with a modern audio library or stub.
- **IPX networking components** – some modules expect IPX APIs. Implement wrappers around modern sockets or stub out for now.

| Library | Example locations | Replacement or stub | Remaining work |
| ------- | ----------------- | ------------------ | --------------- |
| DirectX 5 SDK | `WIN32LIB/SRCDEBUG/DDRAW.CPP`, `WIN32LIB/AUDIO/SOUNDIO.CPP` | `src/ddraw/ddraw_stub.c`, LVGL bridge | Port all graphics and input to portable APIs |
| DirectX Media 5.1 SDK | `CODE/MPGSET.CPP`, `WIN32LIB/MOVIE/MOVIE.CPP` | none | Provide modern video playback or stub |
| Greenleaf Communications Library (GCL) | `CODE/NULLMGR.CPP`, `WIN32LIB/INCLUDE/wincomm.h` | none | Replace serial/comm routines with standard networking |
| HMI SOS | `WIN32LIB/AUDIO/OLSOSDEC.ASM`, `VQ/VQA32/vqaplayp.h` | miniaudio-based timer | Port remaining audio paths |
| IPX components | `CODE/IPXCONN.CPP`, `IPX/` directory | none | Implement socket-based networking |

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
- Removed obsolete `LAUNCH/launch.asm`; `launch_main` now cleans up `.swp` files
  and invokes `game.dat` directly.
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
- Commented out Watcom `#pragma aux` directives in `mpu.h` and `jshell.h` so GCC/Clang builds don't error under `-Werror`.
- Replaced `<io.h>` include in `wwfile.h` with standard `<unistd.h>` for portability.
- Updated `<new.h>` includes to use the standard C++ `<new>` header so Linux builds
  don't fail.
- Introduced a CMake build for the VQA32 playback library and provided C stubs
  for the original assembly routines.
- Added stubs for missing <dos.h> and <pharlap.h> to keep the build going.
- Removed legacy compiler pragmas and compiled VQA32 sources as C++.
- Created portable stubs for the Greenleaf serial functions in `fast.h` and `src/fast_stub.c`.
- Updated legacy VQM headers to compile under C11 by defining the `cdecl` macro when missing.
- Fixed remaining DOS path separators in VQM include files.
- Precomputed `SLUFF_BITS` in `defines.h` to avoid `sizeof` in preprocessor logic.
- Added stubs for missing Windows headers and replaced deprecated APIs in VQA code.
- Implemented a UDP-based IPX stub (`src/ipx_stub.c`) and excluded the
  original real-mode IPX sources and assembly from the build. A simple
  test harness (`ipx_stub_test`) verifies basic send/receive behavior.
- Extended the stub with additional Win95-style functions and basic
  asynchronous ECB semantics so existing IPX manager code can run
  unmodified.
- Refactored the IPX UDP stub to support Windows (Winsock2) and POSIX
  sockets from a single C11 codebase.
- Added C stub blitters and USE_C_BLITTERS option to select them.
- Implemented Buffer_Print, Get_Font_Palette_Ptr and interpolation stubs in C.
- Verified blitter stubs cover functions from KEYFBUFF, TXTPRNT and WINASM.
- Converted Buffer_Frame_To_Page and MCGA_Print from assembly to optimized C and
  compile them automatically when `ENABLE_ASM` is disabled.
- Replaced CPUID.ASM with a portable implementation using __get_cpuid.
- Assembly modules are now assembled with NASM or YASM via CMake when `ENABLE_ASM` is enabled.
- Deleted obsolete `#pragma warning` directives in headers and verified `watcom.h` is no longer included.
- LVGL is now built from the bundled submodule and linked when `USE_LVGL` is enabled.
- Added a minimal `lv_conf.h` to configure the LVGL build.
- LVGL initialization now occurs before other subsystems. `launch_main` and WinMain call `lv_init()` and `lvgl_init_backend()` and the main loop pumps events via `lv_timer_handler()`.
- Implemented C inline versions of `Cardinal_To_Fixed` and `Fixed_To_Cardinal`. `COORDA.ASM` is excluded when `ENABLE_ASM` is OFF.
- Pre-game menus now call `lv_timer_handler()` within their loops so LVGL input devices stay responsive.
- `lvgl_init_backend` now accepts an optional backend name. `launch_main` parses the new `--lvgl-backend=<name>` argument and passes it along, falling back to the `LV_BACKEND` environment variable if the option is missing.
- Implemented a C version of `ModeX_Blit` and used it when assembly blitters are disabled.
- Added runtime logs for each C replacement of assembly modules (ModeX_Blit, interpolation helpers, CPUID detection).
- Added C fallbacks for SUPPORT.ASM, 2SUPPORT.ASM and LCWCOMP.ASM; these stubs build automatically when `ENABLE_ASM` is off.
- Replaced SUPPORT.ASM and 2SUPPORT.ASM with a C11 implementation (`src/support.c`).
- `lvgl_init_backend` now selects the display driver based on the `LV_BACKEND` environment variable and falls back to SDL when unknown.
- `lvgl_init_backend` now logs display creation failures, falls back to the Wayland backend when SDL is unavailable and returns a status code so callers can exit gracefully.
- LVGL backend is selected via the `--lvgl-backend` command-line option or the `LV_BACKEND` environment variable.
- `lvgl_blit` now creates an indexed canvas once and copies the game's frame
  buffer into it every frame using `lv_draw_buf_copy` while updating the
  palette from `CurrentPalette`.
- Menu processing loops now call `lv_timer_handler()` so LVGL widgets stay
  responsive while waiting for user input.
- Added C implementations of MCGAPRIM and SVGAPRIM routines in src/dos_graphics.c. Assembly versions are skipped when ENABLE_ASM is OFF.
- Replaced `LCWCOMP.ASM` with a portable C implementation when `ENABLE_ASM`
  is disabled.
- Added a GitHub action that cross compiles the project for the RV32IMA ILP32 architecture using the X11 backend.
- Added C replacements for Mem_Copy, Largest_Mem_Block and page-in helpers.
- Ported the remaining debug and tool assembly modules to portable C
  (`src/debug/asm_replacements.c` and `tools/audiomak/scode.c`).
  The build now omits `WIN32LIB/SRCDEBUG` and `TOOLS/AUDIOMAK/SCODE.ASM`
  entirely when `ENABLE_ASM` is off.
