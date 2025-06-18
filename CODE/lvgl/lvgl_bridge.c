#include "lvgl_bridge.h"
#include "../../src/lvgl/src/lvgl.h"
#include <stdint.h>
#include <stddef.h>

/*
 * Convert the game's 8-bit GraphicBufferClass surface to an LVGL canvas.
 * This is a minimal placeholder. A real implementation would copy the
 * pixel data and palette into an lv_img_dsc_t or canvas buffer.
 */
void lvgl_blit(const struct GraphicBufferClass *page)
{
    (void)page;
    /* TODO: translate `page` contents into an LVGL canvas */
}

