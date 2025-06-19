#ifndef VQ_LVGL_PLAYER_H
#define VQ_LVGL_PLAYER_H

#include <stdint.h>

typedef struct {
    void *data;
    uint32_t data_size;
    struct {
        uint32_t cf;
        uint32_t w;
        uint32_t h;
    } header;
} lv_img_dsc_t;

lv_img_dsc_t *get_lvgl_frame_desc(void);

#endif /* VQ_LVGL_PLAYER_H */
