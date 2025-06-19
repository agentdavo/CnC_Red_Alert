#include <string.h>
#include <ra/debug_log.h>
#include "drawbuff.h"
#include "gbuffer.h"
#include "mcgaprim.h"
#include "font.h"
#include "vbuffer.h"

#if !ENABLE_ASM
static unsigned char Font_Color_Xlat[256];
static void init_color_xlat(unsigned char fcol, unsigned char bcol)
{
    for (int i = 0; i < 16; ++i)
        Font_Color_Xlat[i] = i;
    for (int g = 1; g < 16; ++g) {
        Font_Color_Xlat[g * 16] = g;
        for (int j = 1; j < 16; ++j)
            Font_Color_Xlat[g * 16 + j] = 0;
    }
    Font_Color_Xlat[0] = bcol;
    Font_Color_Xlat[1] = fcol;
    Font_Color_Xlat[16] = fcol;
}
#endif

#ifdef __cplusplus
extern "C" {
#endif

long Buffer_Size_Of_Region(void *thisptr, int w, int h)
{
    LOG_CALL("Buffer_Size_Of_Region C stub\n");
    return MCGA_Size_Of_Region(thisptr, w, h);
}

void Buffer_Put_Pixel(void *thisptr, int x, int y, unsigned char color)
{
    LOG_CALL("Buffer_Put_Pixel C stub\n");
    MCGA_Put_Pixel(thisptr, x, y, color);
}

int Buffer_Get_Pixel(void *thisptr, int x, int y)
{
    LOG_CALL("Buffer_Get_Pixel C stub\n");
    return MCGA_Get_Pixel(thisptr, x, y);
}

void Buffer_Clear(void *thisptr, unsigned char color)
{
    LOG_CALL("Buffer_Clear C stub\n");
    MCGA_Clear(thisptr, color);
}

long Buffer_To_Buffer(void *thisptr, int x, int y, int w, int h,
                      void *buff, long size)
{
    LOG_CALL("Buffer_To_Buffer C stub\n");
    return MCGA_To_Buffer(thisptr, x, y, w, h, buff, size);
}

long Buffer_To_Page(int x, int y, int w, int h, void *Buffer, void *view)
{
    LOG_CALL("Buffer_To_Page C stub\n");
    return MCGA_Buffer_To_Page(x, y, w, h, Buffer, view);
}



void Buffer_Draw_Line(void *thisptr, int sx, int sy, int dx, int dy,
                      unsigned char color)
{
    LOG_CALL("Buffer_Draw_Line C stub\n");
    MCGA_Draw_Line(thisptr, sx, sy, dx, dy, color);
}

void Buffer_Fill_Rect(void *thisptr, int sx, int sy, int dx, int dy,
                      unsigned char color)
{
    LOG_CALL("Buffer_Fill_Rect C stub\n");
    MCGA_Fill_Rect(thisptr, sx, sy, dx, dy, color);
}

void Buffer_Remap(void *thisptr, int sx, int sy, int width, int height,
                  void *remap)
{
    LOG_CALL("Buffer_Remap C stub\n");
    MCGA_Remap(thisptr, sx, sy, width, height, remap);
}

void Buffer_Fill_Quad(void *thisptr, void *span_buff,
                      int x0, int y0, int x1, int y1,
                      int x2, int y2, int x3, int y3, int color)
{
    LOG_CALL("Buffer_Fill_Quad C stub\n");
    MCGA_Fill_Quad(thisptr, span_buff, x0, y0, x1, y1, x2, y2, x3, y3, color);
}

void Buffer_Draw_Stamp(void const *thisptr, void const *icondata, int icon,
                       int x_pixel, int y_pixel, void const *remap)
{
    LOG_CALL("Buffer_Draw_Stamp C stub\n");
    MCGA_Draw_Stamp(thisptr, icondata, icon, x_pixel, y_pixel, remap);
}

void Buffer_Draw_Stamp_Clip(void const *thisptr, void const *icondata, int icon,
                            int x_pixel, int y_pixel, void const *remap,
                            int minx, int miny, int maxx, int maxy)
{
    LOG_CALL("Buffer_Draw_Stamp_Clip C stub\n");
    MCGA_Draw_Stamp_Clip(thisptr, icondata, icon, x_pixel, y_pixel, remap,
                         minx, miny, maxx, maxy);
}

#if !ENABLE_ASM

LONG Buffer_Print(void *thisptr, const char *str, int x, int y,
                  int fcolor, int bcolor)
{
    LOG_CALL("Buffer_Print C stub\n");
    return MCGA_Print(thisptr, str, x, y, fcolor, bcolor);
}

void *Get_Font_Palette_Ptr(void)
{
    LOG_CALL("Get_Font_Palette_Ptr C stub\n");
    init_color_xlat(0, 0);
    return Font_Color_Xlat;
}

#endif /* !ENABLE_ASM */

 
#ifdef __cplusplus
}
#endif

