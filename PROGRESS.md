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
