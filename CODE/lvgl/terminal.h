#ifndef TERMINAL_H
#define TERMINAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

lv_obj_t * terminal_create(void);
void terminal_add(const char * txt_in);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* TERMINAL_H */
