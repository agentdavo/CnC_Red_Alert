#include "lvgl_backend.h"
#include "../../src/debug_log.h"
#include "../../src/lvgl/src/lvgl.h"
#include "../../src/lvgl/src/drivers/lv_drivers.h"
#include "../externs.h" /* for ScreenWidth/ScreenHeight */
#include <stdlib.h>
#include <string.h>

void lvgl_init_backend(const char *backend)
{
    const char *name = backend;
    if (!name || !*name) {
        name = getenv("LV_BACKEND");
    }
    if (!name || !*name) {
        name = "x11";
    }

    LOG_CALL("Initializing LVGL backend '%s'\n", name);

    lv_display_t *disp = lv_sdl_window_create(ScreenWidth, ScreenHeight);
    lv_sdl_mouse_create();
    lv_sdl_mousewheel_create();
    lv_sdl_keyboard_create();
    lv_display_set_default(disp);
    lv_sdl_window_set_title(disp, "Red Alert");
}
