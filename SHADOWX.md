# Shadow Blit Routine

`Shadow_Blit` is implemented in `WWFLAT32/MCGAPRIM/SHADOW.ASM`. DOS builds call this function from `GScreenClass::Blit_Display` to copy the hidden page to VGA memory.

```
CODE/GSCREEN.CPP:485:               Shadow_Blit(0, 0, 320, 200, HidPage, SeenPage, ShadowPage->Get_Buffer());
```

The routine takes coordinates, width and height, and a pointer to a shadow buffer. It blits each scanline from the source to the destination page, optionally hiding the mouse cursor during the copy.

Windows builds use `ModeX_Blit` or DirectDraw instead.
