# ModeX Blit Routine

`ModeX_Blit` is an assembly routine located in `CODE/WINASM.ASM`. It copies the 320×200 hidden page to the screen when the engine runs in VGA Mode X. Windows builds call this function from `GScreenClass::Blit_Display` when `SeenBuff` has a width of 320 pixels.

```
CODE/GSCREEN.CPP:470:   void ModeX_Blit (GraphicBufferClass * source);
CODE/GSCREEN.CPP:482:                   ModeX_Blit(&HiddenPage);
```

The procedure programs the VGA sequencer (`3C4h`) and writes the four planar memory pages sequentially. It expects a pointer to a `GraphicBufferClass` representing the hidden page.

DOS builds use a different function (`Shadow_Blit`) for the final copy. See `SHADOWX.md` for details.
