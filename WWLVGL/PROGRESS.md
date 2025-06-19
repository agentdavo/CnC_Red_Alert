# Porting Progress

This document tracks the ongoing effort to port the Windows 95 **Command & Conquer: Red Alert** libs to a modern C11 portable code base.

## External dependencies

The original source relies on several legacy libraries that are no longer readily available.
Each of these will need a modern replacement or a temporary stub in order to build the project:

- **DirectX 5 SDK** – handles graphics and input. Replace with a cross-platform rendering library (e.g., SDL, OpenGL) or provide stubs for testing.
- **DirectX Media 5.1 SDK** – used for video playback. Provide an alternative video subsystem or stub implementation.
- **Greenleaf Communications Library (GCL)** – provides networking/communication routines. Substitute with standard networking APIs or remove if unused.
- **Human Machine Interface (HMI) "Sound Operating System" (SOS)** – provides audio support. Replace with a modern audio library or stub.
- **IPX networking components** – some modules expect IPX APIs. Implement wrappers around modern sockets or stub out for now.

| Library | Example locations | Replacement or stub | Remaining work |
| ------- | ----------------- | ------------------ | --------------- |
| DirectX 5 SDK | `WWLVGL/SRCDEBUG/DDRAW.CPP`, `WWLVGL/SRCDEBUG/SOUNDIO.CPP` | LVGL bridge | Port all graphics and input to portable APIs |
| DirectX Media 5.1 SDK | `WWLVGL/MOVIE/MOVIE.CPP` | none | Provide modern video playback or stub |
| Greenleaf Communications Library (GCL) | `WWLVGL/INCLUDE/wincomm.h` | none | Replace serial/comm routines with portable C11 standard networking |
| HMI SOS | `WWLVGL/AUDIO/OLSOSDEC.ASM`, `VQ/LVGL/vq_lvgl_player.c` | miniaudio-based timer | Port remaining audio paths |
| IPX components | `CODE/IPXCONN.CPP`, `IPX/` directory | none | Implement portable C11 socket-based networking |

As the port progresses, updates on how each dependency has been replaced or stubbed should be recorded here.

- Enabled strict build flags in `CMakeLists.txt` so all WWLVGL sources compile with `-std=gnu11 -pedantic -Wall -Wextra -Werror`.
- Switched audio playback to the bundled miniaudio library.
- Began C11 cleanup of MEM module: removed DOS-only headers, added stub `timer.h`, and replaced deprecated includes.
- Replaced legacy MEMFLAG.H with C11 version and added timer stubs for MEM build.

### 2025-06-19
- Renamed MEM/ to mem/ and removed obsolete new.h includes.
- Updated headers for C11 style.
- Build attempted with strict flags; compilation errors remain.

### 2025-06-20
- Lowercased remaining MEM directories in WIN32LIB and WWFLAT32.
- Updated documentation paths to use `mem/`.

### 2025-06-21
- Adjusted CMake to build only the `mem` library while other modules are
  ported to C11.
- Restored `MEM_NEW` flag and added `Add_Long_To_Pointer` helper in
  `memflag.h`.
- Fixed several warnings in `mem.c` and ensured the module builds cleanly
  under strict C11 flags.

### 2025-06-22
- Simplified headers in `mem` sources and unified file comments.
- Updated `memflag.h` prototypes for C11 and added `stdint.h` includes.
- Verified `mem` library builds cleanly with strict C11 flags.
- Attempted to compile remaining WWLVGL folders; build failed due to C++ constructs.

### 2025-06-23
- Added `CMakeLists.txt` to `mem` subdirectory for modular builds.
- Refreshed headers in `mem` sources and removed legacy DOS headers.
- Verified `mem` library compiles cleanly.

### 2025-06-24
- Ran CMake in `build_full/` attempting full WWLVGL compile.
- `mem_lib` built successfully but other modules failed due to Win32 APIs and C++ features.
- Next step: convert remaining `.cpp` files to C and add `CMakeLists.txt` per subfolder.

### 2025-06-25
- Converted `AUDIO` sources to C11 and renamed files to lowercase `.c`.
- Added `audio_lib` with new `CMakeLists.txt` and minimal headers.
- Stubbed legacy SOS structures and removed Win32 dependencies.
- Verified `mem_lib` and `audio_lib` build successfully with strict flags.

### 2025-06-26
- Added `port` module providing `ww_get_drive_type` wrapper and new `port.h` header.
- Replaced Windows `GetDriveType` calls in `PLAYCD/GETCD.CPP` with portable function.
- Removed `<dos.h>` from the file.
- Created `CMakeLists.txt` for every WWLVGL subdirectory and updated top-level
  CMake to include them.
- Removed `WIN32` compile definition from `mem` build.

### 2025-06-27
- Introduced `ww_sleep` and `ww_get_ticks` in `PORT` module.
- Replaced `Sleep` and `GetTickCount` usages in WinComm and memory code.
- Cleaned `PROFILE/UTIL/PROFILE.CPP` and `SRCDEBUG/MONO.CPP` of DOS-only headers.
- Updated `AGENTS.md` with coding guidelines for cross-platform development.

### 2025-06-28
- Restructured SRCDEBUG into module subfolders (AUDIO, RAWFILE, PROFILE, SHAPE, WINCOMM).
- Added CMake targets for each debug module and updated the aggregator.
- Top-level build now links against `srcdebug_lib`.
- Verified compilation succeeds after the restructure.

### 2025-06-29
- Split additional SRCDEBUG modules into subfolders: KEYBOARD, MOUSE, TIMER and VESA.
- Created interface targets for each module and updated the aggregate `srcdebug_lib`.

### 2025-06-30
- Added FILE, BUFFER, FONT, PALETTE and VIDEO subfolders in SRCDEBUG.
- Each module exposes an interface target using GNU++11.
- Created SRCDEBUG/AGENTS.md documenting the C++11 requirement.
