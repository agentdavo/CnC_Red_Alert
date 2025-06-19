#ifndef VQ_LVGL_PLAYER_H
#define VQ_LVGL_PLAYER_H

#include <stdint.h>
#include "../../src/lvgl/src/lvgl.h"
#include "vqaplayp.h"
#include "vq_lvgl_decoder.h"

extern lv_obj_t *img_obj;
extern lv_img_dsc_t *frame_desc;
extern unsigned char *next_frame_data;

lv_img_dsc_t *get_lvgl_frame_desc(void);
void decode_frame_node(VQAHandle *vqa, VQAFrameNode *frame, unsigned char *dest);
void swap_lvgl_buffers(void);
void lvgl_drawer_init(const VQAHeader *header);

#endif /* VQ_LVGL_PLAYER_H */
