# LVGL Integration

An optional build mode sends the game's 8‑bit frames to an [LVGL](https://lvgl.io) canvas instead of relying on DirectDraw.

## CMake option

The top‑level `CMakeLists.txt` defines `USE_LVGL`:

```cmake
option(USE_LVGL "Enable LVGL canvas output" OFF)
```

Configure with `-DUSE_LVGL=ON` to compile the LVGL bridge.

## Bridge functions

`GScreenClass::Blit_Display` calls `lvgl_blit` when the option is active:

```cpp
#ifdef USE_LVGL
        lvgl_blit(&HiddenPage);
#endif
```

The stub implementation appears in `CODE/lvgl/lvgl_bridge.c`:

```c
void lvgl_blit(const struct GraphicBufferClass *page)
{
    (void)page;
    /* TODO: translate `page` contents into an LVGL canvas */
}
```

`lvgl_blit` is expected to run after `lv_init()` has initialized the LVGL library. The official demos illustrate the typical pattern:

```c
lv_init();
...
while (1) {
    uint32_t delay = lv_timer_handler();
    ...
}
```

LVGL's `lv_timer_handler` pumps the GUI and should be called periodically by the launcher.
