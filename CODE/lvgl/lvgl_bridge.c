#include "lvgl_bridge.h"
#include "../../src/lvgl/src/lvgl.h"
#include "../../WWFLAT32/PALETTE/palette.h"
#include <ra/debug_log.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/*
 * Convert the game's 8-bit GraphicBufferClass surface to an LVGL canvas.
 * The routine creates a canvas the first time it's called and reuses it on
 * subsequent frames. Each call updates the palette and copies the frame
 * buffer to the canvas so LVGL can present the image on the selected
 * backend.
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

    LOG_CALL("lvgl_blit start\n");

    /* Lazily create the canvas on the first call */
    static lv_obj_t *canvas = NULL;
    static lv_draw_buf_t *canvas_buf = NULL;

    int w = gbc->view.width;
    int h = gbc->view.height;

    if(canvas == NULL) {
        LOG_CALL("create canvas %dx%d\n", w, h);
        canvas_buf = lv_draw_buf_create(w, h, LV_COLOR_FORMAT_I8, LV_STRIDE_AUTO);
        if(!canvas_buf) return;

        canvas = lv_canvas_create(lv_screen_active());
        lv_canvas_set_draw_buf(canvas, canvas_buf);
        lv_scr_load(canvas);
    }

    /* Update the palette from the game's global palette data */
    LOG_CALL("update palette\n");
    for(int i = 0; i < 256; i++) {
        lv_color32_t col = lv_color32_make(CurrentPalette[i * 3],
                                           CurrentPalette[i * 3 + 1],
                                           CurrentPalette[i * 3 + 2],
                                           0xFF);
        lv_canvas_set_palette(canvas, i, col);
    }

    /* Wrap the game's buffer and copy it to the canvas */
    lv_draw_buf_t src_buf;
    uint32_t stride = gbc->view.width + gbc->view.pitch;
    lv_draw_buf_init(&src_buf, w, h, LV_COLOR_FORMAT_I8, stride,
                     gbc->buf.buffer, stride * h);

    lv_area_t area = {0, 0, w - 1, h - 1};
    LOG_CALL("copy frame to canvas\n");
    lv_draw_buf_copy(canvas_buf, &area, &src_buf, &area);
    lv_obj_invalidate(canvas);
}

