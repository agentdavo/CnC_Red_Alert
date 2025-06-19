#include "../../src/lvgl/src/lvgl.h"
#include "../../src/lvgl/src/draw/lv_image_decoder_private.h"
#include "../../src/lvgl/src/draw/lv_draw_buf_private.h"
#include "vq_lvgl_player.h"
#include "vq_lvgl_decoder.h"

static lv_image_decoder_t *vq_dec;

static lv_result_t vq_decoder_info(lv_image_decoder_t *decoder, lv_image_decoder_dsc_t *dsc, lv_image_header_t *header)
{
    (void)decoder;
    if(dsc->src_type != LV_IMAGE_SRC_VARIABLE) return LV_RESULT_INVALID;
    const vq_raw_image_t *img = dsc->src;
    if(img->magic != VQ_RAW_MAGIC) return LV_RESULT_INVALID;

    header->cf = LV_COLOR_FORMAT_RAW;
    header->w = img->w;
    header->h = img->h;
    return LV_RESULT_OK;
}

static lv_result_t vq_decoder_open(lv_image_decoder_t *decoder, lv_image_decoder_dsc_t *dsc)
{
    (void)decoder;
    if(dsc->src_type != LV_IMAGE_SRC_VARIABLE) return LV_RESULT_INVALID;
    const vq_raw_image_t *img = dsc->src;
    if(img->magic != VQ_RAW_MAGIC) return LV_RESULT_INVALID;

    lv_draw_buf_t *buf = lv_draw_buf_create(img->w, img->h, LV_COLOR_FORMAT_I8, LV_STRIDE_AUTO);
    if(!buf) return LV_RESULT_INVALID;

    lv_memcpy(buf->data, img->data, img->w * img->h);
    buf->header.cf = LV_COLOR_FORMAT_I8;
    dsc->decoded = buf;
    return LV_RESULT_OK;
}

static void vq_decoder_close(lv_image_decoder_t *decoder, lv_image_decoder_dsc_t *dsc)
{
    (void)decoder;
    if(dsc->decoded) lv_draw_buf_destroy((lv_draw_buf_t *)dsc->decoded);
}

void vq_lvgl_decoder_init(void)
{
    if(vq_dec) return;
    vq_dec = lv_image_decoder_create();
    lv_image_decoder_set_info_cb(vq_dec, vq_decoder_info);
    lv_image_decoder_set_open_cb(vq_dec, vq_decoder_open);
    lv_image_decoder_set_close_cb(vq_dec, vq_decoder_close);
}
