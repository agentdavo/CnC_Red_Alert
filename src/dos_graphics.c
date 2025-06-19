#include <string.h>
#include <stdlib.h>
#include "gbuffer.h"
#include "vbuffer.h"
#include "mcgaprim.h"
#include <ra/debug_log.h>

#ifdef __cplusplus
extern "C" {
#endif

long MCGA_Size_Of_Region(void *thisptr, int w, int h)
{
    GraphicViewPortClass *view = (GraphicViewPortClass *)thisptr;
    int width = w < view->Get_Width() ? w : view->Get_Width();
    int height = h < view->Get_Height() ? h : view->Get_Height();
    return (long)width * height;
}

void MCGA_Put_Pixel(void *thisptr, int x, int y, unsigned char color)
{
    GraphicViewPortClass *view = (GraphicViewPortClass *)thisptr;
    if (x < 0 || y < 0 || x >= view->Get_Width() || y >= view->Get_Height())
        return;
    unsigned char *buf = (unsigned char *)view->Get_Graphic_Buffer()->Get_Buffer();
    int pitch = view->Get_Width() + view->Get_XAdd();
    buf += view->Get_Offset() + y * pitch + x;
    *buf = color;
}

int MCGA_Get_Pixel(void *thisptr, int x, int y)
{
    GraphicViewPortClass *view = (GraphicViewPortClass *)thisptr;
    if (x < 0 || y < 0 || x >= view->Get_Width() || y >= view->Get_Height())
        return 0;
    unsigned char *buf = (unsigned char *)view->Get_Graphic_Buffer()->Get_Buffer();
    int pitch = view->Get_Width() + view->Get_XAdd();
    buf += view->Get_Offset() + y * pitch + x;
    return *buf;
}

void MCGA_Clear(void *thisptr, unsigned char color)
{
    GraphicViewPortClass *view = (GraphicViewPortClass *)thisptr;
    unsigned char *dst = (unsigned char *)view->Get_Graphic_Buffer()->Get_Buffer();
    int pitch = view->Get_Width() + view->Get_XAdd();
    dst += view->Get_Offset();
    for (int row = 0; row < view->Get_Height(); ++row) {
        memset(dst + row * pitch, color, view->Get_Width());
    }
}

long MCGA_To_Buffer(void *thisptr, int x, int y, int w, int h, void *buff, long size)
{
    GraphicViewPortClass *view = (GraphicViewPortClass *)thisptr;
    unsigned char *src = (unsigned char *)view->Get_Graphic_Buffer()->Get_Buffer();
    int pitch = view->Get_Width() + view->Get_XAdd();
    src += view->Get_Offset() + y * pitch + x;
    if (w * h > size)
        h = size / w;
    unsigned char *dst = (unsigned char *)buff;
    for (int row = 0; row < h; ++row) {
        memcpy(dst + row * w, src + row * pitch, w);
    }
    return w * h;
}

long MCGA_Buffer_To_Page(int x, int y, int w, int h, void *Buffer, void *viewptr)
{
    GraphicViewPortClass *view = (GraphicViewPortClass *)viewptr;
    unsigned char *dst = (unsigned char *)view->Get_Graphic_Buffer()->Get_Buffer();
    int pitch = view->Get_Width() + view->Get_XAdd();
    dst += view->Get_Offset() + y * pitch + x;
    unsigned char *src = (unsigned char *)Buffer;
    for (int row = 0; row < h; ++row) {
        memcpy(dst + row * pitch, src + row * w, w);
    }
    return 0;
}

static void blit_rect(unsigned char *dst, int dpitch,
                      unsigned char *src, int spitch,
                      int w, int h, int trans)
{
    for (int y = 0; y < h; ++y) {
        if (trans) {
            for (int x = 0; x < w; ++x) {
                unsigned char c = src[y*spitch + x];
                if (c)
                    dst[y*dpitch + x] = c;
            }
        } else {
            memcpy(dst + y*dpitch, src + y*spitch, w);
        }
    }
}

BOOL Linear_Blit_To_Linear(void *srcvp, void *destvp,
                           int sx, int sy, int dx, int dy,
                           int w, int h, BOOL trans)
{
    GraphicViewPortClass *src = (GraphicViewPortClass *)srcvp;
    GraphicViewPortClass *dst = (GraphicViewPortClass *)destvp;
    unsigned char *srcbuf = (unsigned char *)src->Get_Graphic_Buffer()->Get_Buffer();
    unsigned char *dstbuf = (unsigned char *)dst->Get_Graphic_Buffer()->Get_Buffer();
    int spitch = src->Get_Width() + src->Get_XAdd();
    int dpitch = dst->Get_Width() + dst->Get_XAdd();
    srcbuf += src->Get_Offset() + sy * spitch + sx;
    dstbuf += dst->Get_Offset() + dy * dpitch + dx;
    blit_rect(dstbuf, dpitch, srcbuf, spitch, w, h, trans);
    return TRUE;
}

BOOL Linear_Scale_To_Linear(void *srcvp, void *dstvp,
                            int sx, int sy, int dx, int dy,
                            int sw, int sh, int dw, int dh,
                            BOOL trans, char *remap)
{
    GraphicViewPortClass *src = (GraphicViewPortClass *)srcvp;
    GraphicViewPortClass *dst = (GraphicViewPortClass *)dstvp;
    unsigned char *srcbuf = (unsigned char *)src->Get_Graphic_Buffer()->Get_Buffer();
    unsigned char *dstbuf = (unsigned char *)dst->Get_Graphic_Buffer()->Get_Buffer();
    int spitch = src->Get_Width() + src->Get_XAdd();
    int dpitch = dst->Get_Width() + dst->Get_XAdd();
    srcbuf += src->Get_Offset() + sy * spitch + sx;
    dstbuf += dst->Get_Offset() + dy * dpitch + dx;

    for (int y = 0; y < dh; ++y) {
        int syi = (y * sh) / dh;
        for (int x = 0; x < dw; ++x) {
            unsigned char c = srcbuf[syi*spitch + (x * sw) / dw];
            if (remap)
                c = (unsigned char)remap[c];
            if (!trans || c)
                dstbuf[y*dpitch + x] = c;
        }
    }
    return TRUE;
}

void Vesa_Put_Pixel(void *vp, int x, int y, unsigned char color)
{
    MCGA_Put_Pixel(vp, x, y, color);
}

int Vesa_Get_Pixel(void *vp, int x, int y)
{
    return MCGA_Get_Pixel(vp, x, y);
}

void Vesa_Clear(void *vp, unsigned char color)
{
    MCGA_Clear(vp, color);
}

long Vesa_To_Buffer(void *vp, int x, int y, int w, int h, void *buff, long size)
{
    return MCGA_To_Buffer(vp, x, y, w, h, buff, size);
}

long Vesa_Buffer_To_Page(int x, int y, int w, int h, void *buff, void *view)
{
    return MCGA_Buffer_To_Page(x, y, w, h, buff, view);
}

BOOL Linear_Blit_To_Vesa(void *s, void *d, int sx, int sy, int dx, int dy,
                         int w, int h, BOOL t)
{
    return Linear_Blit_To_Linear(s, d, sx, sy, dx, dy, w, h, t);
}

BOOL Vesa_Blit_To_Linear(void *s, void *d, int sx, int sy, int dx, int dy,
                         int w, int h, BOOL t)
{
    return Linear_Blit_To_Linear(s, d, sx, sy, dx, dy, w, h, t);
}

BOOL Vesa_Blit_To_Vesa(void *s, void *d, int sx, int sy, int dx, int dy,
                       int w, int h, BOOL t)
{
    return Linear_Blit_To_Linear(s, d, sx, sy, dx, dy, w, h, t);
}

BOOL Linear_Scale_To_Vesa(void *s, void *d, int sx, int sy, int dx, int dy,
                          int sw, int sh, int dw, int dh, BOOL t, char *r)
{
    return Linear_Scale_To_Linear(s, d, sx, sy, dx, dy, sw, sh, dw, dh, t, r);
}

BOOL Vesa_Scale_To_Linear(void *s, void *d, int sx, int sy, int dx, int dy,
                          int sw, int sh, int dw, int dh, BOOL t, char *r)
{
    return Linear_Scale_To_Linear(s, d, sx, sy, dx, dy, sw, sh, dw, dh, t, r);
}

BOOL Vesa_Scale_To_Vesa(void *s, void *d, int sx, int sy, int dx, int dy,
                        int sw, int sh, int dw, int dh, BOOL t, char *r)
{
    return Linear_Scale_To_Linear(s, d, sx, sy, dx, dy, sw, sh, dw, dh, t, r);
}

LONG Vesa_Print(void *thisptr, const char *str, int x, int y, int f, int b)
{
    return MCGA_Print(thisptr, str, x, y, f, b);
}

VOID MCGA_Draw_Line(void *thisptr, int sx, int sy, int dx, int dy, unsigned char color)
{
    int x = sx;
    int y = sy;
    int w = dx - sx;
    int h = dy - sy;
    int dx1 = w >= 0 ? 1 : -1;
    int dy1 = h >= 0 ? 1 : -1;
    w = abs(w);
    h = abs(h);
    int err = (w > h ? w : -h) / 2;
    while (1) {
        MCGA_Put_Pixel(thisptr, x, y, color);
        if (x == dx && y == dy) break;
        int e2 = err;
        if (e2 > -w) { err -= h; x += dx1; }
        if (e2 <  h) { err += w; y += dy1; }
    }
}

VOID MCGA_Fill_Rect(void *thisptr, int sx, int sy, int dx, int dy, unsigned char color)
{
    if (dx < sx) { int t = dx; dx = sx; sx = t; }
    if (dy < sy) { int t = dy; dy = sy; sy = t; }
    GraphicViewPortClass *view = (GraphicViewPortClass *)thisptr;
    unsigned char *dst = (unsigned char *)view->Get_Graphic_Buffer()->Get_Buffer();
    int pitch = view->Get_Width() + view->Get_XAdd();
    dst += view->Get_Offset() + sy * pitch + sx;
    int w = dx - sx;
    int h = dy - sy;
    for (int row = 0; row < h; ++row) {
        memset(dst + row * pitch, color, w);
    }
}

VOID MCGA_Fill_Quad(void *thisptr, VOID *span_buff, int x0, int y0, int x1, int y1,
                    int x2, int y2, int x3, int y3, int color)
{
    (void)span_buff;
    MCGA_Draw_Line(thisptr, x0, y0, x1, y1, color);
    MCGA_Draw_Line(thisptr, x1, y1, x2, y2, color);
    MCGA_Draw_Line(thisptr, x2, y2, x3, y3, color);
    MCGA_Draw_Line(thisptr, x3, y3, x0, y0, color);
}

VOID MCGA_Remap(void *thisptr, int sx, int sy, int width, int height, VOID *remap)
{
    GraphicViewPortClass *view = (GraphicViewPortClass *)thisptr;
    unsigned char *table = (unsigned char *)remap;
    unsigned char *buf = (unsigned char *)view->Get_Graphic_Buffer()->Get_Buffer();
    int pitch = view->Get_Width() + view->Get_XAdd();
    buf += view->Get_Offset() + sy * pitch + sx;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            buf[y*pitch + x] = table[buf[y*pitch + x]];
        }
    }
}

void MCGA_Draw_Stamp(void const *thisptr, void const *icondata, int icon,
                     int x_pixel, int y_pixel, void const *remap)
{
    (void)icon; (void)remap; (void)icondata;
    MCGA_Put_Pixel((void*)thisptr, x_pixel, y_pixel, 1);
}

void MCGA_Draw_Stamp_Clip(void const *thisptr, void const *icondata, int icon,
                          int x_pixel, int y_pixel, void const *remap,
                          int min_x, int min_y, int max_x, int max_y)
{
    (void)min_x; (void)min_y; (void)max_x; (void)max_y;
    MCGA_Draw_Stamp(thisptr, icondata, icon, x_pixel, y_pixel, remap);
}

int get_clip(void *win, void *rect)
{
    (void)win; (void)rect;
    return 0;
}

int Vesa_Asm_Set_Win(int a)
{
    (void)a;
    return 0;
}

int Vesa_Asm_Next_Win(void)
{
    return 0;
}

#ifdef __cplusplus
}
#endif

