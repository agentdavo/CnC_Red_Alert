#include "lvgl_backend.h"
#include "../../src/lvgl/src/lvgl.h"
#include "../../src/lvgl/src/drivers/lv_drivers.h"
#include "../debug_log.h"
#include "../externs.h" /* for ScreenWidth/ScreenHeight */

int lvgl_init_backend(void)
{
    lv_display_t *disp = NULL;

#if LV_USE_SDL
    disp = lv_sdl_window_create(ScreenWidth, ScreenHeight);
    if(!disp) {
        LOG_MSG("lv_sdl_window_create failed\n");
    } else {
        lv_sdl_mouse_create();
        lv_sdl_mousewheel_create();
        lv_sdl_keyboard_create();
        lv_sdl_window_set_title(disp, "Red Alert");
    }
#endif

#if LV_USE_WAYLAND
    if(disp == NULL) {
        disp = lv_wayland_window_create(ScreenWidth, ScreenHeight, "Red Alert", NULL);
        if(!disp) {
            LOG_MSG("lv_wayland_window_create failed\n");
        }
    }
#endif

    if(!disp) {
        LOG_MSG("LVGL backend initialization failed\n");
        return -1;
    }

    lv_display_set_default(disp);
    return 0;
}
