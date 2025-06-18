#ifndef LVGL_BRIDGE_H
#define LVGL_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

struct GraphicBufferClass;

void lvgl_blit(const struct GraphicBufferClass *page);

#ifdef __cplusplus
}
#endif

#endif /* LVGL_BRIDGE_H */
