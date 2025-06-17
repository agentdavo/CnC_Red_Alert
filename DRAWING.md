# Assembly Rendering and Support Routines

This document summarizes the low level assembly modules used for drawing and related tasks in the original source for **Command & Conquer: Red Alert**.  Each module exposes optimized routines which are called from the C/C++ code to update graphics buffers or perform helper operations.


## Rendering Pipeline

The game uses a double-buffered approach on both DOS and Windows. Two global surfaces are defined in `EXTERNS.H`: `VisiblePage` (also accessed as `SeenBuff`) and `HiddenPage` (`HidPage`). `WinMain` in `STARTUP.CPP` sets up these surfaces with `Set_Video_Mode`, which creates either DirectDraw surfaces or 320×200 Mode X buffers.

During gameplay `Main_Game` repeatedly calls `Main_Loop` in `CONQUER.CPP`. `Main_Loop` processes user input and then invokes `Map.Render()` to draw the next frame. Map ultimately calls `GScreenClass::Render` in `GSCREEN.CPP`. This method sets the logic page to `HidPage`, calls `Draw_It` to draw all objects and UI, and finally calls `Blit_Display` to copy the hidden page to the visible page.

On Windows builds `Blit_Display` uses DirectDraw blits when the screen is 640×400 or higher. If the width is 320 pixels it calls `ModeX_Blit_` from `WINASM.ASM` to copy the back buffer to video memory. DOS builds instead call `Shadow_Blit` to transfer the buffer to VGA memory.

Game elements—tiles, units, UI, and mouse cursor—are thus drawn to `HidPage` each frame using a mix of C++ routines and the assembly helpers listed below. Once drawing is complete the page is flipped and displayed.

## Buffer Blitting

### `KEYFBUFF.ASM` and `2KEYFBUF.ASM`
These files implement `Buffer_Frame_To_Page`, a function that copies an image from a linear buffer to a `GraphicViewPort`.  Numerous flags control its behaviour:

- `FLAG_TRANS`, `FLAG_GHOST`, `FLAG_FADING`, `FLAG_PREDATOR` – enable transparency, ghosting, fading tables or the special "predator" effect.
- Shape bits such as `SHAPE_CENTER` and `SHAPE_PARTIAL` further customise how a shape is drawn.

The routine fetches optional parameters (fading table, translucent table, etc.), adjusts source/destination rectangles and then jumps to specialised inner loops (e.g. `BF_Copy`, `BF_Trans`) to render each line.  It is the core low level blitter used by the game engine.

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

### `COORDA.ASM`
Implements `Cardinal_To_Fixed` and `Fixed_To_Cardinal` for converting integers to 16.16 fixed‑point numbers and back.  These helpers are used for coordinate calculations.

### `LCWCOMP.ASM`
Provides `LCW_Comp`, a Lempel‑type compressor used to shrink animation frames or other binary data before storage.

### `CPUID.ASM`
Detects CPU features and checks for MMX support.  It queries the `CPUID` instruction and sets global flags used by the engine to enable optional routines.

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

