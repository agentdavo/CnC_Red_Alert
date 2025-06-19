#ifndef SURFACE_API_H
#define SURFACE_API_H

#include "../../src/lvgl/src/lvgl.h"
#ifdef __cplusplus
extern "C" {
#endif

struct WWSurface {
    int width;
    int height;
    lv_obj_t *canvas;
    lv_draw_buf_t *draw_buf;
};

void surface_init(struct WWSurface *surf, int w, int h);
void surface_set_palette(struct WWSurface *surf, const unsigned char *palette);
void surface_blit(struct WWSurface *surf, const void *pixels, int pitch);
void surface_flip(struct WWSurface *surf);

#ifdef __cplusplus
}
#endif

#endif /* SURFACE_API_H */
