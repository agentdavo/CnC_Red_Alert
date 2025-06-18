#include "lvgl_backend.h"
#include "../../src/lvgl/src/lvgl.h"
#include "../../src/lvgl/src/drivers/lv_drivers.h"
#include "../externs.h" /* for ScreenWidth/ScreenHeight */
#include <stdlib.h>
#include <string.h>

void lvgl_init_backend(const char *backend)
{
    const char *backend_name = NULL;
    if (backend && backend[0]) {
        backend_name = backend;
    } else {
        backend_name = getenv("LV_BACKEND");
    }

    if (backend_name && backend_name[0]) {
#ifdef _WIN32
        _putenv_s("LV_BACKEND", backend_name);
#else
        setenv("LV_BACKEND", backend_name, 1);
#endif
    }

    lv_display_t *disp = lv_sdl_window_create(ScreenWidth, ScreenHeight);
    lv_sdl_mouse_create();
    lv_sdl_mousewheel_create();
    lv_sdl_keyboard_create();
    lv_display_set_default(disp);
    lv_sdl_window_set_title(disp, "Red Alert");
}
