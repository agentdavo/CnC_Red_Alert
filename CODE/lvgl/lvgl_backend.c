#include "lvgl_backend.h"
#include <ra/debug_log.h>
#include "../../src/lvgl/src/lvgl.h"
#include "../../src/lvgl/src/drivers/lv_drivers.h"
#include "../externs.h" /* for ScreenWidth/ScreenHeight */
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#define strcasecmp _stricmp
#endif

static lv_backend_t current_backend = LV_BACKEND_UNKNOWN;

lv_backend_t lvgl_get_backend(void)
{
    return current_backend;
}

int lvgl_init_backend(const char *backend)
{
    const char *name = backend;
    if(!name || !*name)
        name = getenv("LV_BACKEND");
    if(!name || !*name)
        name = "x11";

    LOG_CALL("Initializing LVGL backend '%s'\n", name);

    lv_display_t *disp = NULL;

    if(strcasecmp(name, "sdl") == 0) {
#if LV_USE_SDL
        disp = lv_sdl_window_create(ScreenWidth, ScreenHeight);
        if(!disp) {
            LOG_MSG("lv_sdl_window_create failed\n");
        } else {
            LOG_CALL("SDL window created\n");
            lv_sdl_mouse_create();
            lv_sdl_mousewheel_create();
            lv_sdl_keyboard_create();
            lv_sdl_window_set_title(disp, "Red Alert");
        }
#endif
    } else if(strcasecmp(name, "wayland") == 0) {
#if LV_USE_WAYLAND
        disp = lv_wayland_window_create(ScreenWidth, ScreenHeight, "Red Alert", NULL);
        if(!disp) {
            LOG_MSG("lv_wayland_window_create failed\n");
        } else {
            LOG_CALL("Wayland window created\n");
        }
#endif
    } else if(strcasecmp(name, "fbdev") == 0) {
#if LV_USE_LINUX_FBDEV
        disp = lv_linux_fbdev_create();
        if(disp) {
            lv_linux_fbdev_set_file(disp, "/dev/fb0");
            LOG_CALL("FBDEV device opened\n");
        }
#endif
    } else {
#if LV_USE_X11
        disp = lv_x11_window_create("Red Alert", ScreenWidth, ScreenHeight);
        if(disp) {
            lv_x11_inputs_create(disp, NULL);
            LOG_CALL("X11 window created\n");
        }
#endif
    }

#if LV_USE_SDL
    if(!disp && strcasecmp(name, "sdl") != 0) {
        LOG_MSG("LVGL backend '%s' failed, falling back to SDL\n", name);
        disp = lv_sdl_window_create(ScreenWidth, ScreenHeight);
        if(disp) {
            LOG_CALL("SDL fallback window created\n");
            lv_sdl_mouse_create();
            lv_sdl_mousewheel_create();
            lv_sdl_keyboard_create();
            lv_sdl_window_set_title(disp, "Red Alert");
            name = "sdl";
        }
    }
#endif

    if(!disp) {
        LOG_MSG("LVGL backend initialization failed\n");
        return -1;
    }

    lv_display_set_default(disp);
    LOG_CALL("LVGL display set as default\n");

    if(strcasecmp(name, "sdl") == 0)
        current_backend = LV_BACKEND_SDL;
    else if(strcasecmp(name, "x11") == 0)
        current_backend = LV_BACKEND_X11;
    else if(strcasecmp(name, "wayland") == 0)
        current_backend = LV_BACKEND_WAYLAND;
    else if(strcasecmp(name, "fbdev") == 0)
        current_backend = LV_BACKEND_FBDEV;
    else
        current_backend = LV_BACKEND_UNKNOWN;

    LV_LOG_INFO("LVGL backend: %s", name);
    return 0;
}
