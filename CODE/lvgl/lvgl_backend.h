#ifndef LVGL_BACKEND_H
#define LVGL_BACKEND_H

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize LVGL using the selected backend. The backend name is taken
 * from the command line or environment. Pass NULL to use the LV_BACKEND
 * environment variable or the default.
 */
void lvgl_init_backend(const char *backend);

#ifdef __cplusplus
}
#endif

#endif /* LVGL_BACKEND_H */
