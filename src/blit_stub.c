#include <string.h>
#include <stdarg.h>
#include "gbuffer.h"
#include "vbuffer.h"
#include "mcgaprim.h"
#include "font.h"

/* Slow C implementations of core assembly blitters */

static unsigned char Font_Color_Xlat[256];
static void init_color_xlat(void)
{
    static int initialized = 0;
    if (!initialized) {
        for (int i = 0; i < 256; ++i)
            Font_Color_Xlat[i] = (unsigned char)i;
        initialized = 1;
    }
}

long MCGA_Buffer_To_Page(int x, int y, int w, int h, void *Buffer, void *viewptr)
{
    GraphicViewPortClass *view = (GraphicViewPortClass *)viewptr;
    unsigned char *dst = (unsigned char *)view->Get_Graphic_Buffer()->Get_Buffer();
    int pitch = view->Get_Width() + view->Get_XAdd();
    unsigned char *src = (unsigned char *)Buffer;
    for (int row = 0; row < h; ++row) {
        memcpy(dst + (y + row) * pitch + x, src + row * w, w);
    }
    return 0;
}

void MCGA_Put_Pixel(void *thisptr, int x, int y, unsigned char color)
{
    GraphicViewPortClass *view = (GraphicViewPortClass *)thisptr;
    unsigned char *dst = (unsigned char *)view->Get_Graphic_Buffer()->Get_Buffer();
    int pitch = view->Get_Width() + view->Get_XAdd();
    dst[(y * pitch) + x] = color;
}

int MCGA_Get_Pixel(void *thisptr, int x, int y)
{
    GraphicViewPortClass *view = (GraphicViewPortClass *)thisptr;
    unsigned char *src = (unsigned char *)view->Get_Graphic_Buffer()->Get_Buffer();
    int pitch = view->Get_Width() + view->Get_XAdd();
    return src[(y * pitch) + x];
}

void MCGA_Clear(void *thisptr, unsigned char color)
{
    GraphicViewPortClass *view = (GraphicViewPortClass *)thisptr;
    unsigned char *dst = (unsigned char *)view->Get_Graphic_Buffer()->Get_Buffer();
    int pitch = view->Get_Width() + view->Get_XAdd();
    for (int row = 0; row < view->Get_Height(); ++row) {
        memset(dst + row * pitch, color, view->Get_Width());
    }
}

long Buffer_Frame_To_Page(int x, int y, int w, int h, void *Buffer, GraphicViewPortClass &view, int flags, ...)
{
    (void)flags; /* ignore transparency options for the stub */
    return MCGA_Buffer_To_Page(x, y, w, h, Buffer, &view);
}

long Buffer_Frame_To_LogicPage(int x, int y, int w, int h, void *Buffer, int flags, ...)
{
    (void)flags;
    extern GraphicViewPortClass *LogicPage;
    return MCGA_Buffer_To_Page(x, y, w, h, Buffer, LogicPage);
}

void ModeX_Blit(GraphicBufferClass *source)
{
    /* Simply copy the source buffer to the visible page */
    extern GraphicBufferClass VisiblePage;
    MCGA_Buffer_To_Page(0, 0, source->Get_Width(), source->Get_Height(),
                        source->Get_Buffer(), &VisiblePage);
}

void Shadow_Blit(long xpix, long ypix, long width, long height,
                 GraphicViewPortClass &src, VideoBufferClass &dst, void *shadowbuff)
{
    (void)shadowbuff;
    unsigned char *srcbuf = (unsigned char *)src.Get_Graphic_Buffer()->Get_Buffer();
    unsigned char *dstbuf = (unsigned char *)dst.Get_Video_Buffer()->Get_Buffer();
    int spitch = src.Get_Width() + src.Get_XAdd();
    int dpitch = dst.Get_Width() + dst.Get_XAdd();
    for (long y = 0; y < height; ++y) {
        memcpy(dstbuf + (ypix + y) * dpitch + xpix,
               srcbuf + (src.Get_YPos() + y) * spitch + src.Get_XPos(),
               width);
    }
}

LONG MCGA_Print(void *thisptr, const char *str, int x, int y, int fcolor, int bcolor)
{
    (void)bcolor;
    GraphicViewPortClass *view = (GraphicViewPortClass *)thisptr;
    while (*str) {
        /* draw a simple rectangle as placeholder for each character */
        for (int cy = 0; cy < 8; ++cy) {
            for (int cx = 0; cx < 6; ++cx) {
                MCGA_Put_Pixel(view, x + cx, y + cy, fcolor);
            }
        }
        x += 6;
        ++str;
    }
    return 0;
}

LONG Buffer_Print(void *thisptr, const char *str, int x, int y, int fcolor, int bcolor)
{
    /* linear surfaces handled the same way for the stub */
    return MCGA_Print(thisptr, str, x, y, fcolor, bcolor);
}

void *Get_Font_Palette_Ptr(void)
{
    init_color_xlat();
    return Font_Color_Xlat;
}

void Asm_Interpolate(unsigned char *src_ptr, unsigned char *dest_ptr,
                     int lines, int src_width, int dest_width)
{
    for (int y = 0; y < lines; ++y) {
        unsigned char *src = src_ptr + y * src_width;
        unsigned char *dst = dest_ptr + (y * 2) * dest_width;
        for (int x = 0; x < src_width; ++x) {
            unsigned char p = src[x];
            dst[2 * x] = p;
            dst[2 * x + 1] = p;
        }
        memcpy(dst + dest_width, dst, src_width * 2);
    }
}

void Asm_Interpolate_Line_Double(unsigned char *src_ptr, unsigned char *dest_ptr,
                                 int lines, int src_width, int dest_width)
{
    Asm_Interpolate(src_ptr, dest_ptr, lines, src_width, dest_width);
}

void Asm_Interpolate_Line_Interpolate(unsigned char *src_ptr, unsigned char *dest_ptr,
                                      int lines, int src_width, int dest_width)
{
    Asm_Interpolate(src_ptr, dest_ptr, lines, src_width, dest_width);
}

void Asm_Create_Palette_Interpolation_Table(void)
{
    extern void Create_Palette_Interpolation_Table(void);
    Create_Palette_Interpolation_Table();
}

