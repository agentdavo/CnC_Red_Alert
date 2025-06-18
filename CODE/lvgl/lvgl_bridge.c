#include "lvgl_bridge.h"
#include "../../src/lvgl/src/lvgl.h"
#include "../../WWFLAT32/PALETTE/palette.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/*
 * Convert the game's 8-bit GraphicBufferClass surface to an LVGL canvas.
 * This is a minimal placeholder. A real implementation would copy the
 * pixel data and palette into an lv_img_dsc_t or canvas buffer.
 */
/*
 * Minimal C representation of the GraphicBufferClass fields that are
 * required here. The actual C++ class has many more members but the
 * layout of the first ones is stable across platforms.
 */
struct gvpc_fields {
    long offset;
    int width;
    int height;
    int xadd;
    int xpos;
    int ypos;
    int pitch;
    int is_direct_draw;
    int lock_count;
    struct GraphicBufferClass *graphic_buff;
};

struct bc_fields {
    void *buffer;
    long size;
    int allocated;
};

struct gbc_fields {
    struct gvpc_fields view;
    struct bc_fields   buf;
};

void lvgl_blit(const struct GraphicBufferClass *page)
{
    const struct gbc_fields *gbc = (const struct gbc_fields *)page;

    /* Lazily create the canvas on the first call */
    static lv_obj_t *canvas = NULL;
    static lv_draw_buf_t *canvas_buf = NULL;

    int w = gbc->view.width;
    int h = gbc->view.height;

    if(canvas == NULL) {
        canvas_buf = lv_draw_buf_create(w, h, LV_COLOR_FORMAT_I8, LV_STRIDE_AUTO);
        if(!canvas_buf) return;

        canvas = lv_canvas_create(lv_screen_active());
        lv_canvas_set_draw_buf(canvas, canvas_buf);
        lv_scr_load(canvas);
    }

    /* Update the palette from the game's global palette data */
    for(int i = 0; i < 256; i++) {
        lv_color32_t col = lv_color32_make(CurrentPalette[i * 3],
                                           CurrentPalette[i * 3 + 1],
                                           CurrentPalette[i * 3 + 2],
                                           0xFF);
        lv_canvas_set_palette(canvas, i, col);
    }

    /* Wrap the game's buffer so lv_canvas_copy_buf can read from it */
    lv_draw_buf_t src_buf;
    uint32_t stride = gbc->view.width + gbc->view.pitch;
    lv_draw_buf_init(&src_buf, w, h, LV_COLOR_FORMAT_I8, stride,
                     gbc->buf.buffer, stride * h);

    lv_area_t area = {0, 0, w - 1, h - 1};
    lv_canvas_copy_buf(canvas, &area, &src_buf, &area);
}

