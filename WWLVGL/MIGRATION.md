## Windows.h usage

The following WWLVGL files still include `windows.h` and depend on Win32 APIs.
Each will need a portable replacement.

- `AUDIO/SOUNDIO.CPP` – DirectSound playback routines.
- `AUDIO/SOUNDINT.CPP` – Decompresses audio blocks for playback.
- `AUDIO/SOUNDLCK.CPP` – Locks sound buffers and callbacks.
- `AUDIO/TEST.CPP`, `AUDIO/TST.CPP` – Small audio test programs.
- `AUDIO/OLD/SOUNDIO.CPP`, `AUDIO/OLD/SOUNDINT.CPP`, `AUDIO/OLD/SOUNDLCK.CPP` – Prior revision of the DirectSound code.
- `AUDIO/OLD/TEST.CPP`, `AUDIO/OLD/TST.CPP` – Older test harnesses.
- `DRAWBUFF/ICONCACH.CPP` – Icon caching using Win32 handles.
- `DRAWBUFF/TEST/TEST.CPP`, `DRAWBUFF/TEST/TEST.BAK` – Drawing test code.
- `INCLUDE/misc.h`, `MISC/misc.h` – Helper macros that pull in `<windows.h>`.
- `INCLUDE/modemreg.h`, `WINCOMM/modemreg.h` – Registry access for modem settings.
- `INCLUDE/rawfile.h`, `RAWFILE/rawfile.h`, `RAWFILE/RAWFILE.CPP` – File I/O wrappers around Win32 APIs.
- `INCLUDE/timer.h`, `TIMER/timer.h` – Multimedia timer abstraction.
- `INCLUDE/wincomm.h`, `WINCOMM/wincomm.h` – Serial communication routines.
- `INCLUDE/wwstd.h`, `MISC/wwstd.h` – Miscellaneous utilities relying on Win32.
- `MEM/MSVC/MEM.CPP` – Memory management helpers built for MSVC.
- `PROFILE/PROFILE.CPP`, `PROFILE/WPROFILE.CPP` – Profiling helpers using Windows counters.
- `SRCDEBUG/*.CPP` – Debug builds of the above modules.
- `WW_WIN/OLD/WINDOWS.CPP`, `WW_WIN/OLD/WINHIDE.CPP` – Basic Win32 window creation and hiding.
