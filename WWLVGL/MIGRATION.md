## Windows.h References

The following files include `windows.h` and still rely on Win32 APIs. Each will need a portable alternative as part of the migration process.

- `AUDIO/SOUNDIO.CPP` – main audio output routines.
- `AUDIO/TST.CPP` – audio test harness.
- `AUDIO/SOUNDINT.CPP` – internal sound helpers.
- `AUDIO/SOUNDLCK.CPP` – sound synchronization utilities.
- `AUDIO/OLD/SOUNDIO.CPP` – legacy audio output module.
- `AUDIO/OLD/TST.CPP` – legacy audio test.
- `AUDIO/OLD/SOUNDINT.CPP` – old sound helpers.
- `AUDIO/OLD/SOUNDLCK.CPP` – old locking helpers.
- `AUDIO/OLD/TEST.CPP` – historical test program.
- `AUDIO/TEST.CPP` – modern audio test program.
- `DRAWBUFF/ICONCACH.CPP` – icon caching for Windows UI.
- `DRAWBUFF/TEST/TEST.BAK` – backup of drawing test source.
- `DRAWBUFF/TEST/TEST.CPP` – drawing buffer test source.
- `INCLUDE/wincomm.h` – Win32 communication declarations.
- `INCLUDE/wwstd.h` – Win32 specific helpers.
- `INCLUDE/modemreg.h` – modem registry helpers.
- `INCLUDE/misc.h` – miscellaneous Win32 utilities.
- `INCLUDE/rawfile.h` – Win32 file helper header.
- `INCLUDE/timer.h` – timer interface using Win32 calls.
- `WINCOMM/wincomm.h` – communication port implementation.
- `WINCOMM/modemreg.h` – modem registry implementation.
- `RAWFILE/rawfile.h` – header for raw file access.
- `RAWFILE/RAWFILE.CPP` – implementation of raw file I/O.
 - `mem/MSVC/MEM.CPP` – memory manager based on Win32.
- `SRCDEBUG/ICONCACH.CPP` – debug icon cache module.
- `SRCDEBUG/PROFILE.CPP` – debug profiling helpers.
- `SRCDEBUG/RAWFILE.CPP` – debug raw file routines.
- `SRCDEBUG/SOUNDIO.CPP` – debug sound output.
- `SRCDEBUG/TST.CPP` – debug audio test.
- `SRCDEBUG/SOUNDINT.CPP` – debug sound internals.
- `SRCDEBUG/SOUNDLCK.CPP` – debug locking helpers.
- `SRCDEBUG/WPROFILE.CPP` – debug wide-character profiling.
- `SRCDEBUG/TEST.CPP` – debug test harness.
- `PROFILE/PROFILE.CPP` – profiling helpers.
- `PROFILE/WPROFILE.CPP` – wide-character profiling helpers.
- `MISC/wwstd.h` – miscellaneous utilities with Win32.
- `MISC/misc.h` – header with Win32 specific types.
- `TIMER/timer.h` – timer API built on Win32.
- `WW_WIN/OLD/WINHIDE.CPP` – window management wrapper.
- `WW_WIN/OLD/WINDOWS.CPP` – original Windows interface layer.

Replace these Windows dependencies with portable C11 or cross‑platform libraries to enable builds on other operating systems.
