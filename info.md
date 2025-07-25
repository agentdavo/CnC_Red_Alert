# Directory Overview

This document summarizes the purpose and contents of several legacy folders included with the Red Alert source release.  These directories hold utilities, video playback libraries, and portions of the original Westwood development framework.  They are largely written for Watcom or Borland toolchains and many rely on 16‑bit assembly or old Windows APIs.

## TOOLS

`TOOLS` contains small utility programs used during game development. Most entries are precompiled executables (`.EXE`) alongside their source.  Notable subdirectories include:

- **AUDIOMAK/** – Source for `AUDIOMAK.EXE`, a tool that builds audio data.  The header in `AUDIOMAK.CPP` shows it was written by Joe Bostic in 1994【F:TOOLS/AUDIOMAK/AUDIOMAK.CPP†L1-L22】.
- **MIX/** – Holds the `MIXFILE.EXE` tool used to create Westwood `.MIX` archives.  Its source is `MIXFILE.CPP`【F:TOOLS/MIX/MIXFILE.CPP†L1-L11】.

Other executables (e.g., `ANIMATE.EXE`, `FONTMAKE.EXE`) are provided without accompanying code.  `AUDIOMAK` ships with a Borland style `MAKEFILE` showing how the tool was built using `bcc` and `tasm`【F:TOOLS/AUDIOMAK/MAKEFILE†L18-L32】.  The `MIXFILE` source contains extensive error handling comments detailing how data files are verified before insertion into an archive【F:TOOLS/MIX/MIXFILE.CPP†L290-L339】.

## VQ

`VQ` is a video playback library.  It is divided into several components:

- **INCLUDE/** – Header files grouped into `HMI32`, `VQA32`, `VQM32`, and `WWLIB32`. These define data structures for the Westwood video codec and the HMI SOS sound system.
- **LIB/** – Documentation describing the library naming scheme.  Each compiled library name encodes its target compiler and memory model【F:VQ/LIB/README.TXT†L1-L10】.
- **VQA32/** – C/C++ source that handles playing `.VQA` video streams.
- **VQM32/** – Lower level video codec implementation, including assembly routines and compression helpers.

The VQ headers describe compile-time options for the stand‑alone player. For example `vqaplay.h` lists flags enabling audio, video and MCGA support【F:VQ/INCLUDE/VQA32/vqaplay.h†L24-L53】.  The main audio module defines dummy marker functions and global timer state variables used during playback【F:VQ/VQA32/AUDIO.CPP†L90-L110】.

## WIN32LIB

`WIN32LIB` provides a Win32 version of Westwood’s base library.  It is organized into many subdirectories (e.g., `AUDIO`, `FONT`, `Iff`, `KEYBOARD`).  The root `MAKEFILE` enumerates all component libraries that form the final `win32lib` archive【F:WIN32LIB/MAKEFILE†L90-L111】. Build scripts such as `MAKE.BAT` expect the Watcom compiler in a DOS-style path【F:WIN32LIB/MAKE.BAT†L1-L2】.

The `MAKEFILE` lists install and update targets for each sublibrary from `audio.lib` through `wincomm.lib`【F:WIN32LIB/MAKEFILE†L88-L132】.  The batch file merely sets the `WATCOM` variable then invokes `wmake` with the `WIN32=1` flag.

## WINVQ

`WINVQ` is a Windows port of the VQ video tools.  It mirrors the layout of `VQ` but includes additional Windows viewer utilities:

- **INCLUDE/** – Headers for VQA and VQM in a Win32 environment.
- **LIB/** – Prebuilt HMI SOS libraries with the same naming conventions as the DOS version【F:WINVQ/LIB/README.TXT†L1-L10】.
- **VPLAY32/** – Source for a small player program (`PLYVQA32.CPP`) that demonstrates video playback.
- **VQAVIEW/** – A Windows viewer with dialog resources and support code.
- **VQA32/** and **VQM32/** – Win32 equivalents of the DOS codec sources.

The sample player `VPLAY32` uses a Watcom makefile defining environment variables such as `PRJDIR` and `WATCOM`【F:WINVQ/VPLAY32/MAKEFILE†L20-L40】【F:WINVQ/VPLAY32/MAKEFILE†L58-L64】.  The viewer resources in `VQAVIEW` include a dialog definition file with standard GPL headers【F:WINVQ/VQAVIEW/DIALOGS.RC†L1-L20】.

## WWFLAT32

`WWFLAT32` holds the original flat-memory version of Westwood's C/ASM runtime.  Many of these modules later evolved into `WIN32LIB`.  Key areas include:

- **AUDIO/** – Assembly codecs and SOS interface code used for in-game sound.
- **FILE/** – File system wrappers such as `FILE.CPP` and `FILELIB.CPP`.
- **MCGAPRIM/** – Low-level graphics primitives written largely in assembly.
- **PROJECT.CFG** – Defines Watcom compiler flags for this environment【F:WWFLAT32/PROJECT.CFG†L1-L8】.

The library makefiles describe required environment variables such as `WWFLAT`, `WWVCS` and `WATCOM` and verify them with `!error` directives【F:WWFLAT32/AUDIO/MAKEFILE†L20-L56】.  This directory still contains many assembly modules and stub implementations that were later replaced in the Win32 library.

Additional folders cover fonts, palettes, shape rendering, timers, and more.  Some directories contain backup files (`*.BAK`) and old assembly listings, highlighting the early DOS origins of this code base.

