#ifndef WINDOWSX_H
#define WINDOWSX_H
#include <stdint.h>
#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp) ((int16_t)((uint32_t)(lp) & 0xFFFF))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp) ((int16_t)(((uint32_t)(lp) >> 16) & 0xFFFF))
#endif
#endif /* WINDOWSX_H */
