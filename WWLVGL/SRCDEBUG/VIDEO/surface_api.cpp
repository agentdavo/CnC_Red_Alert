#include "../../INCLUDE/surface_api.h"
#include <ra/debug_log.h>

void surface_init(struct WWSurface *surf, int w, int h)
{
    if(!surf) return;
    surf->width = w;
    surf->height = h;
    surf->draw_buf = lv_draw_buf_create(w, h, LV_COLOR_FORMAT_I8, LV_STRIDE_AUTO);
    if(!surf->draw_buf) {
        LOG_MSG("lv_draw_buf_create failed");
        return;
    }
    surf->canvas = lv_canvas_create(lv_screen_active());
    lv_canvas_set_draw_buf(surf->canvas, surf->draw_buf);
    lv_scr_load(surf->canvas);
}

void surface_set_palette(struct WWSurface *surf, const unsigned char *palette)
{
    if(!surf || !palette || !surf->canvas) return;
    for(int i = 0; i < 256; ++i) {
        lv_color32_t c = lv_color32_make(palette[i*3], palette[i*3+1], palette[i*3+2], 0xFF);
        lv_canvas_set_palette(surf->canvas, i, c);
    }
}

void surface_blit(struct WWSurface *surf, const void *pixels, int pitch)
{
    if(!surf || !pixels || !surf->draw_buf) return;
    lv_draw_buf_t src;
    lv_draw_buf_init(&src, surf->width, surf->height, LV_COLOR_FORMAT_I8, pitch, (void*)pixels, pitch * surf->height);
    lv_area_t a = {0,0,surf->width-1,surf->height-1};
    lv_draw_buf_copy(surf->draw_buf, &a, &src, &a);
}

void surface_flip(struct WWSurface *surf)
{
    if(!surf || !surf->canvas) return;
    lv_obj_invalidate(surf->canvas);
    lv_timer_handler();
}
