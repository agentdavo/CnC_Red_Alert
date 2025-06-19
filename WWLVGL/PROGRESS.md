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
