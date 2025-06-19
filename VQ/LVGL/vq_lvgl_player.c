#include "vq_lvgl_player.h"

static lv_img_dsc_t frame_desc;

lv_img_dsc_t *get_lvgl_frame_desc(void)
{
    return &frame_desc;
}
