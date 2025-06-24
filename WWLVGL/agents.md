# AGENTS Instructions

This folder contains the original sources for the Window 95 library functions of **Command & Conquer: Red Alert**.

The long-term goal is to port the code to standard C11 and port to alternatives and remove its reliance on ALL Windows-only libraries.

## Scope

These instructions apply to all files within this folder only.

## Porting Guidelines

- Prefer C11-compliant code. Avoid C++ features unless absolutely necessary.
- Replace legacy Windows APIs (e.g., DirectX, HMI SOS) with open source portable libraries or stub implementations.
- Keep assembly modules intact until a C equivalent is available.
- Document notable changes and dependency replacements in `progress.md`.
- After each modification, run available build or test scripts where possible.

Use this file as a quick reference when planning code conversions or dependency replacements within this folder.
## Coding Guidelines

- Prefer forward slashes in `#include` directives (`<sys/types.h>` rather than `<sys\types.h>`).
- Avoid `<dos.h>`, `<conio.h>`, and direct Win32 headers except in isolated portability stubs.
- Replace Win32 functions such as `Sleep` or `GetTickCount` with the wrappers in `PORT/port.c` (`ww_sleep`, `ww_get_ticks`).
- Stick to the C11 standard library: `<stdint.h>`, `<stdbool.h>`, `<time.h>` and similar headers.

## Iterative Development

When working on this folder iteratively:

- Build the WWLVGL sources after each change using either `CMake` or `MAKE.BAT`.
- Fix any compilation issues before proceeding to the next change.
- Document each step in `progress.md` so progress can be tracked.
- Some build scripts (for example `MAKE.BAT`) invoke tools from the `CODE/` directory.
  Keep these cross-directory paths valid or update them if the layout changes.

## Reference Documentation

Several Markdown files in this folder contain critical details about the original library.
Consult these documents when porting or refactoring code - update them if anything is wrong or needs clarification.

- `agents.md` – this document.
- `README.md` – intro.
- `drawing.md` – explanation of the rendering pipeline and assembly helpers.
- `sound.md` – breakdown of the audio subsystem and DirectSound routines.
- `ddraw.md` – lists DirectDraw usage across the code base.
- `lvgl.md` – documents the LVGL integration.
- `keyboard.md` – notes on the keyboard handler and LVGL keyboard integration.
- `mouse.md` – notes on the mouse system and LVGL input devices.
- `progress.md` – location where you document work completed per PR.

### **C11-Compliance Quick-Check (apply to every legacy source file)**

| #      | Category                             | What to look for                                                                                                                  | Typical remedy                                                                                                              |
| ------ | ------------------------------------ | --------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------- |
| **1**  | **Undeclared identifiers**           | Loop counters or temporaries used before they are declared.                                                                       | Declare the variable in the smallest possible scope (e.g., `for (int i = 0; …)` or `int i;` at top of block).               |
| **2**  | **Non-standard library calls**       | Functions that exist only on old compilers: `stricmp`, `strnicmp`, `kbhit`, `conio.h`, etc.                                       | Replace with portable C11 or POSIX equivalents (`strcasecmp`, `fcntl`, etc.) or add a small wrapper in a portability layer. |
| **3**  | **Malformed `switch` statements**    | Plain labels used without the `case` keyword, duplicated `case`s, or missing `default`.                                           | Ensure every label is `case <CONST>:`, add `default:` where sensible.                                                       |
| **4**  | **Pre-processor mismatches**         | `#if SYMBOL` opened with one macro and closed with a different one; blocks that don’t nest correctly.                             | Make sure `#if / #ifdef / #ifndef` pairs match the *same* macro and include an explanatory comment (`/* FEATURE_X */`).     |
| **5**  | **Sentinel initialisers**            | Using `(void*)0` (`NULL`) to initialise *integer* or *enum* fields, or vice-versa.                                                | Use `0` for integers, `NULL` for pointers, or an explicit cast.                                                             |
| **6**  | **Wrong `printf` / `scanf` formats** | `%lx` used for signed `long`, `%d` for `size_t`, etc.                                                                             | Match format specifier to exact type (`%li`, `%zu`, `%u`, …).                                                               |
| **7**  | **Implicit function declarations**   | Calling a function before a visible prototype (removed after C90).                                                                | Include the header that declares it or add a prototype yourself.                                                            |
| **8**  | **Structure copying pitfalls**       | `memcpy` or `memset` on structs containing function pointers or padding.                                                          | Prefer simple assignment (`dest = src;`) or designated initialisers.                                                        |
| **9**  | **Missing standard headers**         | Using library facilities without including the correct header (`memcpy` w/o `<string.h>`, `EXIT_SUCCESS` w/o `<stdlib.h>`, etc.). | Add the standard header include close to other includes.                                                                    |
| **10** | **Magic numbers / literal flags**    | Hard-coded constants for buffer counts, bit masks, error codes.                                                                   | Replace with named `enum`s or `#define` constants.                                                                          |
| **11** | **Obsolete keywords or extensions**  | `far`, `huge`, `__based`, old DOS pragmas, K\&R parameter lists.                                                                  | Remove or wrap in `#ifdef LEGACY_COMPILER` blocks.                                                                          |
| **12** | **Thread-unsafe globals**            | File-scope variables used as scratch storage.                                                                                     | Pass data through parameters or mark as `static _Thread_local` if unavoidable.                                              |

---

### **Modernisation Opportunities (once the code compiles)**

1. **Designated initialisers** – safer struct setup:

   ```c
   VQAConfig cfg = {
       .DrawerCallback = NULL,
       .Vmode          = MCGA,
       .ImageWidth     = 320,
       .ImageHeight    = 200,
       .OptionFlags    = VQAOPTF_AUDIO,
   };
   ```

2. **`static const` instead of `#define`-array sentinels** for tables.

3. **`enum` types for option masks** (use `1u << n` and `typedef` the mask).

4. **`_Static_assert`** to confirm table sizes and struct offsets at compile time.

5. **Replace manual string parsing with `<ctype.h>` / `<inttypes.h>` helpers** (`strtoimax`, `strtoumax`) for robust INI reading.

6. **Use `stdbool.h` and `true/false`** rather than string compares for booleans.

7. **Move platform quirks into a single “port.h” header** so the bulk of the code is clean C11.

8. **Adopt `-std=c11 -pedantic -Wall -Wextra -Werror` in your CI/build**, forcing every file through the same strict gate.

9. **Gradually introduce unit tests** for each module while you touch it; catching regressions is easier than hunting them later.

10. **Document each legacy function while you modernise** – even a one-line Doxygen block helps the next engineer.

Run the checklist first to get every file compiling cleanly; then apply the modernisation items.