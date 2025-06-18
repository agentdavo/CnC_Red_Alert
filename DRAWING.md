# Assembly Rendering and Support Routines

This document summarizes the low level assembly modules used for drawing and related tasks in the original source for **Command & Conquer: Red Alert**.  Each module exposes optimized routines which are called from the C/C++ code to update graphics buffers or perform helper operations.


## Rendering Pipeline

The game uses a double-buffered approach on both DOS and Windows. Two global surfaces are defined in `EXTERNS.H`: `VisiblePage` (also accessed as `SeenBuff`) and `HiddenPage` (`HidPage`). `WinMain` in `STARTUP.CPP` sets up these surfaces with `Set_Video_Mode`, which creates either DirectDraw surfaces or 320×200 Mode X buffers.

During gameplay `Main_Game` repeatedly calls `Main_Loop` in `CONQUER.CPP`. `Main_Loop` processes user input and then invokes `Map.Render()` to draw the next frame. Map ultimately calls `GScreenClass::Render` in `GSCREEN.CPP`. This method sets the logic page to `HidPage`, calls `Draw_It` to draw all objects and UI, and finally calls `Blit_Display` to copy the hidden page to the visible page.

On Windows builds `Blit_Display` uses DirectDraw blits when the screen is 640×400 or higher. If the width is 320 pixels it calls `ModeX_Blit_` from `WINASM.ASM` to copy the back buffer to video memory. DOS builds instead call `Shadow_Blit` to transfer the buffer to VGA memory.

Game elements—tiles, units, UI, and mouse cursor—are thus drawn to `HidPage` each frame using a mix of C++ routines and the assembly helpers listed below. Once drawing is complete the page is flipped and displayed.

## High-Level Rendering Functions

- **`WinMain`** – entry point in `STARTUP.CPP` that initializes DirectDraw and calls `Main_Game`.
- **`Main_Game`** – creates the main loop and repeatedly invokes `Main_Loop` until the user quits.
- **`Main_Loop`** – processes input and then triggers `Map.Render()` to build the next frame.
- **`DisplayClass::Draw_It`** – refreshes map cells and draws world objects to the logic page.
- **`GScreenClass::Render`** – sets the logic page, calls `Draw_It`, draws UI buttons, and finally calls `Blit_Display`.
- **`ObjectClass::Render`** – virtual function implemented by individual objects to draw themselves.
- **`GScreenClass::Blit_Display`** – performs the page flip using DirectDraw, Mode X or `Shadow_Blit`.

## Buffer Blitting

### `KEYFBUFF.ASM` and `2KEYFBUF.ASM`
These files implement `Buffer_Frame_To_Page`, a function that copies an image from a linear buffer to a `GraphicViewPort`.  Numerous flags control its behaviour:

- `FLAG_TRANS`, `FLAG_GHOST`, `FLAG_FADING`, `FLAG_PREDATOR` – enable transparency, ghosting, fading tables or the special "predator" effect.
- Shape bits such as `SHAPE_CENTER` and `SHAPE_PARTIAL` further customise how a shape is drawn.

The routine fetches optional parameters (fading table, translucent table, etc.), adjusts source/destination rectangles and then jumps to specialised inner loops (e.g. `BF_Copy`, `BF_Trans`) to render each line.  It is the core low level blitter used by the game engine.

Other entry points include `Buffer_Frame_To_LogicPage` for copying directly to the active logic page and a series of `WSA_*` helpers (`WSA_Normal_Draw`, `Transparent_Draw`, `Priority_Draw`, `Priority_Transparent_Draw`, `Ghost_Normal_Draw`, `Ghost_Transparent_Draw`, `Ghost_Priority_Draw`, and `Ghost_Priority_Transparent_Draw`) that handle Westwood's WSA animation files.

## Text Rendering

### `TXTPRNT.ASM`
Provides `MCGA_Print` for drawing text characters to a `GraphicViewPort` when running in MCGA/Mode X.  The function locates font data blocks, verifies that text fits in the viewport, then renders each glyph pixel-by-pixel using a colour translation table.

`Get_Font_Palette_Ptr` returns a pointer to that 256‑byte palette table.

### `2TXTPRNT.ASM`
A slightly different version named `Buffer_Print`.  It performs the same operations but includes an additional pitch value for drawing directly to DirectDraw surfaces.

## Helper Routines

### `SUPPORT.ASM` and `2SUPPORT.ASM`
Contain several small helpers used throughout the engine:

- **`Fat_Put_Pixel`** – draws a square "pixel" of arbitrary size directly into a graphics buffer, faster than filling a rectangle.
- **`Conquer_Build_Fading_Table`** – constructs custom fading tables so repeated shadows do not accumulate brightness.  The routine scans a palette and chooses the best matching colours for the fade target.
- **`Remove_From_List`** – removes an element from a pointer list by shifting remaining entries down.
- **`Square_Root`** – integer square root for fixed‑point values.
- **`strtrim`** – trims trailing whitespace from a string.
- **`Get_EAX`** – returns the current EAX register value (utility for debugging).

### `COORDA.ASM`
Implements `Cardinal_To_Fixed` and `Fixed_To_Cardinal` for converting integers to 16.16 fixed‑point numbers and back.  These helpers are used for coordinate calculations.

### `LCWCOMP.ASM`
Provides `LCW_Comp`, a Lempel‑type compressor used to shrink animation frames or other binary data before storage.

### `CPUID.ASM`
Contains `Detect_MMX_Availability`, which calls the `CPUID` instruction to detect CPU features and set the global flags used by the engine for MMX acceleration.

### `WINASM.ASM`
Contains a collection of Windows‑specific helpers:

- **`ModeX_Blit_`** – copies the back buffer to video memory when running in 320×200 Mode X.
- **Interpolation routines** – `Asm_Interpolate`, `Asm_Interpolate_Line_Double`, and `Asm_Interpolate_Line_Interpolate` upscale a 320×200 frame to 640×400 using palette interpolation.
- **`Set_Palette_Register_`** – writes colour values directly to VGA DAC registers.
- Additional modem and palette helpers used by the Win32 port.

## IPX Support Modules

`IPXPROT.ASM` and `IPXREAL.ASM` contain real‑mode stubs for IPX networking.  They provide functions such as `Get_RM_IPX_Address` and the interrupt handler `IPXHandler`.  These are not involved in rendering but are part of the assembly code base.

## Usage

The C++ code calls these routines through `extern "C"` declarations.  For example, `GScreenClass::Blit_Display` invokes `ModeX_Blit_` when the screen is 320×200, while map rendering logic uses `Buffer_Frame_To_Page` for sprite blits.  Many of the assembly modules remain until portable C equivalents are written.


## Graphics Setup Functions

`InitDDraw` in `STARTUP.CPP` initializes DirectDraw and allocates the `VisiblePage` and `HiddenPage` surfaces. These buffers are attached to the `SeenBuff` and `HidPage` viewports with `Set_Video_Mode` so all rendering uses a double buffer.

## WIN32LIB Drawing Library

The `WIN32LIB/DRAWBUFF` directory provides additional assembly helpers used by `GraphicViewPortClass`:

- **`Buffer_Put_Pixel`** and **`Buffer_Get_Pixel`** – write or read a single pixel.
- **`Buffer_Clear`** – fill a viewport with a color.
- **`Buffer_Draw_Line`** – Bresenham-style line routine.
- **`Buffer_Fill_Rect`** and **`Buffer_Fill_Quad`** – draw rectangles or convex polygons.
- **`Linear_Blit_To_Linear`** – copy rectangular areas between buffers.
- **`Linear_Scale_To_Linear`** – stretch or shrink a rectangle while copying.
- **`Buffer_To_Page`** and **`Buffer_To_Buffer`** – move raw image data into a viewport or another buffer.
- **`Buffer_Draw_Stamp`** and **`Buffer_Draw_Stamp_Clip`** – stamp cached icons with optional clipping.
- **`Buffer_Remap`** – apply palette remapping tables.
- **`Buffer_Size_Of_Region`** – return the dimensions of a clipped region.
- **`get_clip`** – calculate a clipping rectangle from window coordinates.
- **`Init_Stamps`** – prepares stamp tables before drawing icons.
- **`Shadow_Blit`** – copies the hidden page to VGA memory when running in DOS (implemented in `MCGAPRIM/SHADOW.ASM`).

Mouse support in `WIN32LIB/KEYBOARD` uses assembly routines `Draw_Mouse` and `Erase_Mouse` to render the cursor image each frame.

## Object-Level Drawing Functions

Many gameplay classes override a `Draw_It` method to paint themselves onto the
logic page.  Each implementation typically resides in the class’s source file:

- `UnitClass::Draw_It` – `CODE/UNIT.CPP`
- `InfantryClass::Draw_It` – `CODE/INFANTRY.CPP`
- `AircraftClass::Draw_It` – `CODE/AIRCRAFT.CPP`
- `VesselClass::Draw_It` – `CODE/VESSEL.CPP`
- `BulletClass::Draw_It` – `CODE/BULLET.CPP`
- `TerrainClass::Draw_It` – `CODE/TERRAIN.CPP`
- `OverlayTypeClass::Draw_It` – `CODE/ODATA.CPP`
- `SmudgeTypeClass::Draw_It` – `CODE/SDATA.CPP`

User interface code also relies on `IconCacheClass::Draw_It` found in
`WIN32LIB/DRAWBUFF/ICONCACH.CPP` for drawing cached icons.
Text printing uses the helper `Fancy_Text_Print` declared in
`CODE/TEXTBTN.CPP` and called throughout modules such as `CODE/RADAR.CPP` and
`CODE/CREDITS.CPP`.

## Graphic Buffer Methods

`GraphicViewPortClass` (implemented in `WIN32LIB/DRAWBUFF/GBUFFER.CPP`) exposes
high-level routines that wrap the assembly blitters:

- `Blit` – copy a viewport or video buffer (`GBUFFER.H` lines 271–278).
- `Clear` – call `Buffer_Clear` to fill a region.
- `Fill_Rect` – invoke `Buffer_Fill_Rect`.
- `Draw_Line` – wrapper around `Buffer_Draw_Line`.
- `Draw_Stamp` – call `Buffer_Draw_Stamp` or its clipped variant.

These C++ methods provide a portable interface while the heavy lifting occurs
in the assembly routines listed above.

## Detailed Rendering Flow

The entry point `WinMain` allocates the main window and then calls `InitDDraw` to create `IDirectDraw` interfaces. `Set_Video_Mode` chooses the display resolution and attaches `VisiblePage` and `HiddenPage` as DirectDraw surfaces. These become the `SeenBuff` and `HidPage` viewports used by the renderer.

```
WinMain
  -> InitDDraw
    -> Set_Video_Mode(width, height, 8)
      -> CreatePalette / CreateSurface
  -> Main_Game(argc, argv)
```

Inside `Main_Game` the loop `Main_Loop` runs until exit. Each pass performs game logic, calls `Map.Render()` and finally flips pages. `GScreenClass::Render` is responsible for calling `Draw_It`, drawing the UI, and then invoking `Blit_Display`.

### Page Flipping

`Blit_Display` checks the screen width. For 640×400 or 640×480 it calls `HidPage.Blit` which wraps a DirectDraw blit from the hidden surface to the visible one. When the screen is exactly 320×200 it instead calls the assembly routine `ModeX_Blit_` to copy the planar buffer to VGA memory. DOS builds skip DirectDraw entirely and use `Shadow_Blit` to write directly to A0000h.

### CPU Features and MMX

The game tests for x86 MMX capability using `CPUID.ASM`. If MMX is available the flag `MMXAvailable` is set. Some blitters in `2KEYFBUF.ASM` can branch to `MMX_Done` for accelerated loops, though the reference code still falls back to 32‑bit integer operations if MMX is absent. The CPUID routine also stores the CPU vendor string and type for debugging.

### Mode X Blitter Details

`ModeX_Blit_` programs the VGA sequencer via port `3C4h` to select each of the four planes. It iterates over 200 lines, copying 8 pixels per iteration with unrolled MOV instructions. The destination is video memory at segment `A0000h`. This low‑level routine mirrors the hidden page to the screen without using DirectDraw.

### DirectDraw Surfaces

DirectDraw surfaces are managed through the `WIN32LIB` wrappers. `VisiblePage` and `HiddenPage` each hold an `IDirectDrawSurface` pointer obtained during `InitDDraw`. When `Get_Caps` indicates enough memory, `HiddenPage` is also allocated in VRAM and attached to `VisiblePage` with `Attach_DD_Surface` for hardware page flipping. Otherwise a system memory surface is used and the blit occurs through the CPU.

### Mouse Rendering

The mouse cursor is drawn every frame via `WWMouseClass::Draw_Mouse`, implemented in `WIN32LIB/KEYBOARD/WWMOUSE.ASM`. It reads the current mouse shape, hides the previous image, and plots the new cursor using direct writes to the target viewport. DOS versions rely on `VGA_Draw_Mouse` and `VESA_Draw_Mouse` implementations that interface with real‑mode code.
Additional helpers in the same file include `Mouse_Shadow_Buffer` for saving the pixels under the cursor and `ASM_Set_Mouse_Cursor` which updates the hardware cursor image when DirectDraw allows it.

### Video Modes and Interpolation

For movie playback and certain cut‑scenes the game can scale 320×200 content to 640×400. The assembly files provide interpolation helpers (`Asm_Interpolate`, `Asm_Interpolate_Line_Double`, and `Asm_Interpolate_Line_Interpolate`) which interpolate color indices between neighbouring pixels to create a smoother scaled frame.


## Source File Reference

### CODE/STARTUP.CPP
- `WinMain` – Windows entry point that initializes audio and video with `InitDDraw`, sets `VisiblePage` and `HiddenPage` via `Set_Video_Mode`, then calls `Main_Game`.
- `InitDDraw` – Creates DirectDraw surfaces and configures global pages.
- `Set_Video_Mode` – Chooses resolution and attaches surfaces.

### CODE/CONQUER.CPP
- `Main_Game` – Configures the scenario and runs `Main_Loop`.
- `Main_Loop` – Processes input and each frame calls `Map.Render()`.

### CODE/GSCREEN.CPP
- `GScreenClass::Render` – Sets the logic page, draws objects and UI, then calls `Blit_Display`.
- `GScreenClass::Blit_Display` – Copies `HiddenPage` to `VisiblePage`, using `ModeX_Blit_` or `Shadow_Blit` when necessary.
- `Set_Logic_Page` – Selects which viewport subsequent drawing targets.

### CODE/UNIT.CPP
- `UnitClass::Draw_It` – Renders vehicle sprites, turret rotation and recoil.

### CODE/INFANTRY.CPP
- `InfantryClass::Draw_It` – Draws infantry frames according to stance.

### CODE/AIRCRAFT.CPP
- `AircraftClass::Draw_It` – Handles aircraft tilt and rotor animation.

### CODE/VESSEL.CPP
- `VesselClass::Draw_It` – Renders naval units and submerged sprites.

### CODE/BULLET.CPP
- `BulletClass::Draw_It` – Renders projectiles and tracer lines.

### CODE/TERRAIN.CPP
- `TerrainClass::Draw_It` – Paints terrain tiles to the logic page.

### CODE/SDATA.CPP
- `SmudgeTypeClass::Draw_It` – Overlays scorch marks and craters.

### WIN32LIB/DRAWBUFF/GBUFFER.CPP
- `GraphicViewPortClass::Blit`, `Clear`, `Fill_Rect`, `Draw_Line`, `Draw_Stamp` – C++ wrappers around the assembly blitters in `DRAWBUFF`.

### CODE/KEYFBUFF.ASM
- `Buffer_Frame_To_Page` – Core blit routine with transparency and ghosting flags.

### CODE/2KEYFBUF.ASM
- Alternate `Buffer_Frame_To_Page` with optional MMX acceleration.

### CODE/TXTPRNT.ASM and CODE/2TXTPRNT.ASM
- `MCGA_Print` and `Buffer_Print` – Output text in Mode X or linear buffers.

### WIN32LIB/KEYBOARD/WWMOUSE.ASM
- `WWMouseClass::Draw_Mouse`, `Erase_Mouse`, `Mouse_Shadow_Buffer`, `ASM_Set_Mouse_Cursor` – Mouse cursor rendering helpers.
## Summary

Drawing relies on a mix of C++ and 32‑bit x86 assembly. DirectDraw surfaces are used for high‑resolution Windows modes while a specialized Mode X path handles the classic 320×200 resolution. The game keeps a hidden page for rendering and shows it via blits or page flips each frame. Optimized routines handle text output, sprite blits, palette updates and cursor drawing, with optional MMX acceleration detected at startup.
