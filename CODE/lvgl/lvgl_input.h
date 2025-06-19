#ifndef LVGL_INPUT_H
#define LVGL_INPUT_H

#ifdef __cplusplus
extern "C" {
#endif

void lvgl_input_init(void);
void lvgl_input_push_key(unsigned int key, int pressed);
void lvgl_input_set_mouse_pos(int x, int y);
void lvgl_input_push_mouse_button(unsigned int button, int pressed);

#ifdef __cplusplus
}
#endif

#endif /* LVGL_INPUT_H */
