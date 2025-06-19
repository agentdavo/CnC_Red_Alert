# WWLVGL Directory Overview

The WWLVGL directory contains cross-platform LVGL-based runtime libraries based on Westwood’s existing WIN32LIB/

The WWLVGL/CMakeLists.txt file enumerates the component libraries that build the final archive:

- misc.lib
- mono.lib
- palette.lib
- shape.lib
- tile.lib
- timer.lib
- ww_win.lib
- wsa.lib
- profile.lib
- playcd.lib
- wincomm.lib

## Subdirectory Overview

### AUDIO/
The miniaudio driver implementation (SOUNDIO.CPP) and old assembly decompressors like AUDUNCMP.ASM and OLSOSDEC.ASM. The `Audio_Init` routine creates a sound object and sets up the primary buffer. `sound.h` exposes constants such as `MAX_SFX` and `SECONDARY_BUFFER_SIZE`.

### DIPTHONG/
Provides text compression/decompression helpers (DIPTHONG.CPP, _DIPTABL.CPP) used for resource strings.

### DRAWBUFF/
Graphics buffer system implemented mostly in assembly (BITBLIT.ASM, DRAWLINE.ASM). Includes C/C++ wrappers (BUFFER.CPP) and headers like `gbuffer.h`.

### EXAMPLE/
Small sample application demonstrating library usage (MAIN.CPP, STARTUP.CPP).

### FONT/
Font loader and rendering helpers. Contains assembly for setting palette entries (SETFPAL.ASM, TEXTPRNT.ASM).

### IFF/
Image I/O and compression modules (LCWCOMP.ASM, LCWUNCMP.ASM) with high-level loaders (LOADPCX.CPP).

### INCLUDE/
Shared headers: drawing macros, keyboard structures, memory utilities, and other .INC files referenced by the source.

### KEYBOARD/
Keyboard and mouse input handlers. The old directory contains real-mode assembly drivers (KEYIREAL.ASM), while the main code uses Windows APIs.

### mem/
Memory allocators and wrappers, plus assembly helpers like MEM_COPY.ASM and VMPAGEIN.ASM.

### MISC/
Miscellaneous utilities including DDRAW.CPP, which manages draw surfaces, declaring globals.

### MONO/
Simple text output to a monochrome debug window (MONO.CPP).

### MOVIE/
Video playback code (MOVIE.CPP).

### PALETTE/
Palette management routines with an assembly palette blitter (PAL.ASM).

### PLAYCD/
CD audio driver using Windows MCI (PLAYCD.ASM, REDBOOK.CPP).

### PROFILE/
Profiling utilities with an assembly interface (APROFILE.ASM) and C wrappers.

### RAWFILE/
File I/O abstraction for raw data streams (RAWFILE.CPP, CCFILE.CPP).

### SHAPE/
Sprite rendering with many assembly drawing routines (e.g., DRAWSHP.ASM, DS_TABLE.ASM).

### SRCDEBUG/
Debug versions of all modules (both assembly and C/C++) for step-through builds.

### TILE/
Tile set support (ICONSET.CPP) for user interface icons.

### TIMER/
Timer utilities (TIMER.CPP, TIMERDWN.CPP) that originally hooked the Windows multimedia timer.

### WINCOMM/
Serial/modem communication wrapper (WINCOMM.CPP).

### WSA/
Westwood Animation format support (WSA.CPP, XORDELTA.ASM).

### WW_WIN/
Basic window setup functions for the application (WINDOWS.CPP, WINHIDE.CPP).

## Migration Status
`MIGRATION.md` lists remaining dependencies on Windows APIs. Items include DirectDraw routines in `SRCDEBUG/DDRAW.CPP` and DirectSound playback in `AUDIO/SOUNDIO.CPP`. Later entries note that assembly graphics and audio routines have been ported to C and replaced with portable implementations when `ENABLE_ASM` is disabled.

## Coding Tasks to Convert WWLVGL to Portable C11

1. **Catalog Windows dependencies.** Audit each subdirectory (e.g., MISC/DDRAW.CPP, AUDIO/SOUNDIO.CPP) and document which functions rely on DirectX, Win32 window management, or serial APIs. Record findings in `PROGRESS.md`.

2. **Convert C++ sources to C.** Replace constructors, destructors, and other C++ features in DRAWBUFF, RAWFILE, and other modules with plain C functions and structures.

3. **Replace DirectDraw usage.** Introduce a C11 graphics abstraction (backed by LVGL or SDL) that offers surface creation, blitting, and palette operations, then rewrite DDRAW.CPP and related drawing code to use this API.

4. **Swap DirectSound with miniaudio.** Port SOUNDIO.CPP and SOUNDLCK.CPP to open miniaudio devices instead of DirectSound objects. Remove HWND parameters from `Audio_Init` and use cross-platform callbacks.

5. **Reimplement timers.** Replace Windows multimedia timer calls in TIMER.CPP with a portable timer wrapper that works on POSIX and embedded platforms.

6. **Refactor file I/O.** Consolidate RAWFILE and CCFILE into portable stdio-based routines, eliminating references to Windows handles and DOS paths.

7. **Port input handling.** Change keyboard and mouse modules to use LVGL input devices or SDL events, discarding the real-mode assembly and Windows message hooks.

8. **Translate assembly graphics helpers.** Convert blitters in DRAWBUFF and SHAPE to optimized C11 functions. Keep the assembly files for reference until feature parity is achieved.

9. **Remove Greenleaf modem code.** Replace WINCOMM with a thin POSIX socket layer and stub modem calls so networked play can use standard TCP/UDP.

10. **Update the build system.** Create CMake targets for each library, compiling them as C11 sources. Gradually eliminate Watcom-specific options from `PROJECT.CFG` and the makefiles. Document the new build process and remaining TODO items in `PROGRESS.md`.

These steps will transform WWLVGL into a portable C11 library, free from Windows-only dependencies and assembly requirements.
