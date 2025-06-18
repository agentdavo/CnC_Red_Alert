#ifndef LVGL_BACKEND_H
#define LVGL_BACKEND_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    LV_BACKEND_SDL,
    LV_BACKEND_X11,
    LV_BACKEND_WAYLAND,
    LV_BACKEND_FBDEV,
    LV_BACKEND_UNKNOWN
} lv_backend_t;

int lvgl_init_backend(const char *backend);
lv_backend_t lvgl_get_backend(void);

#ifdef __cplusplus
}
#endif

#endif /* LVGL_BACKEND_H */
