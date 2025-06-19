#include "vq_lvgl_player.h"
#include "../../src/lvgl/src/lvgl.h"
#include "vqaplayp.h"
#include "vq_lvgl_decoder.h"
#include "../VQM32/compress.h"
#include "vq.h"
#include "unvq.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ra/debug_log.h>

lv_obj_t *img_obj;
static lv_img_dsc_t frame_desc_storage;
lv_img_dsc_t *frame_desc = &frame_desc_storage;
unsigned char *next_frame_data;

lv_img_dsc_t *get_lvgl_frame_desc(void)
{
    return frame_desc;
}

void decode_frame_node(VQAHandle *vqa, VQAFrameNode *frame, unsigned char *dest)
{
    LOG_CALL("decode_frame_node\n");
    if(!frame) return;

    VQAData *vqabuf = ((VQAHandleP *)vqa)->VQABuf;
    VQADrawer *drawer = &vqabuf->Drawer;
    VQAFrameNode *save = drawer->CurFrame;

    drawer->CurFrame = frame;
    if(frame->Codebook->Flags & VQACBF_CBCOMP) {
        LCW_Uncompress((char *)frame->Codebook->Buffer + frame->Codebook->CBOffset,
                       (char *)frame->Codebook->Buffer, vqabuf->Max_CB_Size);
        frame->Codebook->Flags &= ~VQACBF_CBCOMP;
    }
    if(frame->Flags & VQAFRMF_PALCOMP) {
        frame->PaletteSize = LCW_Uncompress((char *)frame->Palette + frame->PalOffset,
                                            (char *)frame->Palette, vqabuf->Max_Pal_Size);
        frame->Flags &= ~VQAFRMF_PALCOMP;
    }
    if(frame->Flags & VQAFRMF_PTRCOMP) {
        LCW_Uncompress((char *)frame->Pointers + frame->PtrOffset,
                       (char *)frame->Pointers, vqabuf->Max_Ptr_Size);
        frame->Flags &= ~VQAFRMF_PTRCOMP;
    }

    vqabuf->UnVQ(frame->Codebook->Buffer, frame->Pointers,
                 dest + sizeof(vq_raw_image_t),
                 drawer->BlocksPerRow, drawer->NumRows, drawer->ImageWidth);

    drawer->CurFrame = save;
}

void swap_lvgl_buffers(void)
{
    LOG_CALL("swap_lvgl_buffers\n");
    unsigned char *tmp = (unsigned char *)frame_desc->data;
    frame_desc->data = next_frame_data;
    next_frame_data = tmp;

    vq_raw_image_t *desc_raw = (vq_raw_image_t *)frame_desc->data;
    desc_raw->data = (const uint8_t *)(desc_raw + 1);

    vq_raw_image_t *next_raw = (vq_raw_image_t *)next_frame_data;
    next_raw->data = (const uint8_t *)(next_raw + 1);

    lv_img_set_src(img_obj, frame_desc);
}

void lvgl_drawer_init(const VQAHeader *header)
{
    LOG_CALL("lvgl_drawer_init\n");
    size_t sz;

    frame_desc->header.cf = LV_COLOR_FORMAT_RAW;
    frame_desc->header.w = header->ImageWidth;
    frame_desc->header.h = header->ImageHeight;

    sz = header->ImageWidth * header->ImageHeight;
    frame_desc->data_size = sizeof(vq_raw_image_t) + sz;
    frame_desc->data = malloc(frame_desc->data_size);
    next_frame_data = malloc(frame_desc->data_size);

    vq_raw_image_t *raw = (vq_raw_image_t *)frame_desc->data;
    raw->magic = VQ_RAW_MAGIC;
    raw->w = header->ImageWidth;
    raw->h = header->ImageHeight;
    raw->data = (const uint8_t *)(raw + 1);
    memcpy(next_frame_data, frame_desc->data, sizeof(vq_raw_image_t));

    img_obj = lv_img_create(lv_screen_active());
    lv_img_set_src(img_obj, frame_desc);
    vq_lvgl_decoder_init();
}
