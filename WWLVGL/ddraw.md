# DirectDraw Usage

This document lists where `ddraw.h` in WWLVGL/ is  included and summarizes calls to the DirectDraw API.

These references need replacement or shims for portable C11 builds.

## Files including ddraw.h

```
WINVQ/VQAVIEW/MAIN.CPP:46:#include <ddraw.h>
CODE/movie.h:38:#include <ddraw.h>
WWLVGL/MOVIE/movie.h:37:#include <ddraw.h>
WWLVGL/INCLUDE/gbuffer.h:113:#include <ddraw.h>
WWLVGL/INCLUDE/misc.h:46:#include <ddraw.h>
WWLVGL/MISC/misc.h:46:#include <ddraw.h>
WWLVGL/DRAWBUFF/TEST/TEST.CPP:61:#include <ddraw.h>
WWLVGL/DRAWBUFF/TEST/TEST.BAK:61:#include <ddraw.h>
WWLVGL/DRAWBUFF/ICONCACH.CPP:56:#include "ddraw.h"
WWLVGL/DRAWBUFF/gbuffer.h:113:#include <ddraw.h>
WWLVGL/SRCDEBUG/ICONCACH.CPP:56:#include "ddraw.h"
```

## Example DirectDraw calls

```
CODE/STATS.CPP:592:             if (DirectDrawObject){
CODE/STATS.CPP:594:                     if (DD_OK == DirectDrawObject->GetCaps (&video_capabilities , NULL)){
CODE/TOOLTIP.CPP:223:   //      Saves a rect of the LogicPage DirectDraw surface to pBits.
CODE/DISPLAY.CPP:1755:                          if (HidPage.Get_IsDirectDraw()){
CODE/CONQUER.CPP:3049:          DirectDrawObject->SetCooperativeLevel(MainWindow, DDSCL_NORMAL);
CODE/CONQUER.CPP:3060:          DirectDrawObject->SetCooperativeLevel(MainWindow, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
CODE/CONQUER.CPP:3066:          IDirectDrawSurface* primary = NULL;
CODE/CONQUER.CPP:3070:          if (FAILED(DirectDrawObject->SetDisplayMode(ScreenWidth, ScreenHeight, 16)))
CODE/CONQUER.CPP:3082:                  if (FAILED(DirectDrawObject->CreateSurface(&ddsd, &primary, NULL)))
CODE/CONQUER.CPP:3093:                          MpgPlay(filename, DirectDrawObject, primary, &rect);
CODE/CONQUER.CPP:3100:                  DirectDrawObject->SetDisplayMode(ScreenWidth, ScreenHeight, 8);
CODE/CONQUER.CPP:3115:         static IDirectDrawPalette* _palette = NULL;
CODE/CONQUER.CPP:3147:                          if (FAILED(PaletteSurface->SetPalette((IDirectDrawPalette*)data)))
...
WWLVGL/MOVIE/movie.h:42:              Movie(IDirectDraw* dd);
WWLVGL/MOVIE/movie.h:46:              bool Play(IDirectDrawSurface* surface);
WWLVGL/KEYBOARD/TEST/TEST.CPP:80:extern       LPDIRECTDRAW    DirectDrawObject;
```

There are over two hundred DirectDraw references across the project; the full `grep -R "DirectDraw"` output lists them all. Most reside in `WWLVGL/MISC/DDRAW.CPP`, which sets video modes and manages surfaces, and in `CODE/CONQUER.CPP` for movie playback.

The newly added shim under `src/ddraw` provides stub implementations so the code can be compiled without the original DirectDraw headers.

## Additional findings

- `WWLVGL/MISC/DDRAW.CPP` houses most of the DirectDraw initialization and palette management. It creates `DirectDrawObject`, sets display modes and queries hardware capabilities. These routines should call the LVGL back-end or become no-ops.
- `gbuffer.h` (and its copy in `DRAWBUFF/`) maintains an `IsDirectDraw` flag and stores an `LPDIRECTDRAWSURFACE` for each `GraphicBufferClass`. Many modules (`DISPLAY.CPP`, `INTERPAL.CPP`, `EGOS.CPP`, `RADAR.CPP`) check this flag before locking or blitting. With LVGL the flag can remain `false` so all blits occur in system memory.
- `ICONCACH.CPP` allocates DirectDraw surfaces for cached icons. The cache can be rewritten to use plain memory when porting.
- Movie playback headers (`CODE/movie.h`, `WWLVGL/MOVIE/movie.h`) rely on `IDirectDraw` handles. LVGL builds should deliver decoded frames to `lvgl_blit` instead of creating DirectDraw surfaces.
- The stub in `src/ddraw` only defines a minimal subset of the API; additional methods may need stubs as more code is compiled.

