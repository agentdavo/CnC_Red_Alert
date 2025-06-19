#ifndef VQ_LVGL_DECODER_H
#define VQ_LVGL_DECODER_H

typedef struct {
    uint32_t magic;
    uint16_t w;
    uint16_t h;
    const uint8_t *data;
} vq_raw_image_t;

#define VQ_RAW_MAGIC 0x56515241u

void vq_lvgl_decoder_init(void);

#endif /* VQ_LVGL_DECODER_H */
