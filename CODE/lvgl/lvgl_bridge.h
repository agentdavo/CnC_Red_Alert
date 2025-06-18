#ifndef LVGL_BRIDGE_H
#define LVGL_BRIDGE_H

#include "../../src/lvgl/lvgl.h"

int lvgl_bridge_init(int width, int height);
void lvgl_bridge_deinit(void);
extern lv_obj_t *canvas;

#endif /* LVGL_BRIDGE_H */
