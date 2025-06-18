#include "lvgl_bridge.h"
#include <stdlib.h>

lv_obj_t *canvas = NULL;
static void *canvas_buf = NULL;

int lvgl_bridge_init(int width, int height)
{
#if USE_LVGL
    size_t buf_size = (size_t)width * (size_t)height;
    canvas_buf = lv_malloc(buf_size);
    if(!canvas_buf) {
        return -1;
    }
    canvas = lv_canvas_create(lv_scr_act());
    if(!canvas) {
        lv_free(canvas_buf);
        canvas_buf = NULL;
        return -1;
    }
    lv_canvas_set_buffer(canvas, canvas_buf, width, height, LV_COLOR_FORMAT_I8);
#else
    (void)width;
    (void)height;
#endif
    return 0;
}

void lvgl_bridge_deinit(void)
{
#if USE_LVGL
    if(canvas) {
        lv_obj_del(canvas);
        canvas = NULL;
    }
    if(canvas_buf) {
        lv_free(canvas_buf);
        canvas_buf = NULL;
    }
#endif
}
