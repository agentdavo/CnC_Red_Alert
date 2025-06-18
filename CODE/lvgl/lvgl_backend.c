#include "lvgl_backend.h"
#include "../../src/lvgl/src/lvgl.h"
#include "../../src/lvgl/src/drivers/lv_drivers.h"
#include "../externs.h" /* for ScreenWidth/ScreenHeight */
#include <stdlib.h>
#include <string.h>

void lvgl_init_backend(void)
{
    const char *name = getenv("LV_BACKEND");
    if(!name || *name == '\0') name = "x11";

    lv_backend_t backend = LV_BACKEND_UNKNOWN;
    if(strcasecmp(name, "sdl") == 0) backend = LV_BACKEND_SDL;
    else if(strcasecmp(name, "x11") == 0) backend = LV_BACKEND_X11;
    else if(strcasecmp(name, "wayland") == 0) backend = LV_BACKEND_WAYLAND;
    else if(strcasecmp(name, "fbdev") == 0) backend = LV_BACKEND_FBDEV;

    lv_display_t *disp = NULL;

    switch(backend) {
    case LV_BACKEND_WAYLAND:
        disp = lv_wayland_window_create(ScreenWidth, ScreenHeight, "Red Alert", NULL);
        break;
    case LV_BACKEND_FBDEV:
        disp = lv_linux_fbdev_create();
        if(disp) lv_linux_fbdev_set_file(disp, "/dev/fb0");
        break;
    case LV_BACKEND_X11:
        disp = lv_x11_window_create("Red Alert", ScreenWidth, ScreenHeight);
        if(disp) lv_x11_inputs_create(disp, NULL);
        break;
    case LV_BACKEND_SDL:
        disp = lv_sdl_window_create(ScreenWidth, ScreenHeight);
        if(disp) {
            lv_sdl_mouse_create();
            lv_sdl_mousewheel_create();
            lv_sdl_keyboard_create();
        }
        break;
    default:
        break;
    }

    if(!disp) {
        LV_LOG_WARN("Unknown LV_BACKEND '%s', falling back to SDL", name);
        disp = lv_sdl_window_create(ScreenWidth, ScreenHeight);
        if(disp) {
            lv_sdl_mouse_create();
            lv_sdl_mousewheel_create();
            lv_sdl_keyboard_create();
        }
        name = "sdl";
    }

    lv_display_set_default(disp);

    if(strcmp(name, "sdl") == 0) {
        lv_sdl_window_set_title(disp, "Red Alert");
    }

    LV_LOG_INFO("LVGL backend: %s", name);
}
