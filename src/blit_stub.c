#include <string.h>
#include <stdarg.h>
#include "gbuffer.h"
#include "vbuffer.h"
#include "mcgaprim.h"
#include "font.h"

/* Slow C implementations of core assembly blitters */

static unsigned char Font_Color_Xlat[256];
static void init_color_xlat(unsigned char fcol, unsigned char bcol)
{
    /*
     * Replicates the lookup table used by the original assembly routines.
     * Index 0..15 are identity mappings.  Each higher group of 16 bytes
     * stores the value of the high nibble in the first slot and zeros in
     * the remainder.  Entries for the foreground/background colours are
     * patched at runtime.
     */
    for (int i = 0; i < 16; ++i) {
        Font_Color_Xlat[i] = i;
    }
    for (int g = 1; g < 16; ++g) {
        Font_Color_Xlat[g * 16] = g;
        for (int j = 1; j < 16; ++j)
            Font_Color_Xlat[g * 16 + j] = 0;
    }

    Font_Color_Xlat[0] = bcol;
    Font_Color_Xlat[1] = fcol;
    Font_Color_Xlat[16] = fcol;
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
    unsigned char *src = (unsigned char *)Buffer;
    unsigned char *dst_base = (unsigned char *)view.Get_Graphic_Buffer()->Get_Buffer();
    int pitch = view.Get_Width() + view.Get_XAdd();
    unsigned char *dst = dst_base + view.Get_Offset() + y * pitch + x;

    unsigned char *is_trans = NULL;
    unsigned char *trans_table = NULL;
    unsigned char *fade_table = NULL;
    int fade_count = 0;

    va_list args;
    va_start(args, flags);
    if (flags & SHAPE_GHOST) {
        is_trans = va_arg(args, unsigned char *);
        trans_table = is_trans ? is_trans + 256 : NULL;
    }
    if (flags & SHAPE_FADING) {
        fade_table = va_arg(args, unsigned char *);
        fade_count = va_arg(args, int);
    }
    va_end(args);

    for (int row = 0; row < h; ++row) {
        unsigned char *s = src + row * w;
        unsigned char *d = dst + row * pitch;
        for (int col = 0; col < w; ++col) {
            unsigned char px = s[col];
            if ((flags & SHAPE_TRANS) && px == 0)
                continue;

            if (fade_table) {
                for (int f = 0; f < fade_count; ++f)
                    px = fade_table[px];
            }

            if (is_trans) {
                unsigned char idx = is_trans[px];
                if (idx != 0xFF)
                    px = trans_table[(idx << 8) + d[col]];
            }

            d[col] = px;
        }
    }

    return 0;
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
    if (!FontPtr || !str)
        return 0;

    GraphicViewPortClass *view = (GraphicViewPortClass *)thisptr;
    int vpw = view->Get_Width();
    int vph = view->Get_Height();
    int pitch = vpw + view->Get_XAdd();
    unsigned char *base = (unsigned char *)view->Get_Graphic_Buffer()->Get_Buffer();
    unsigned char *curline = base + view->Get_Offset() + y * pitch;
    unsigned char *draw = curline + x;

    unsigned char *font = (unsigned char *)FontPtr;
    unsigned char *info = font + *(unsigned short *)(font + FONTINFOBLOCK);
    unsigned char *off_tbl = font + *(unsigned short *)(font + FONTOFFSETBLOCK);
    unsigned char *width_tbl = font + *(unsigned short *)(font + FONTWIDTHBLOCK);
    unsigned char *height_tbl = font + *(unsigned short *)(font + FONTHEIGHTBLOCK);
    unsigned char maxh = info[FONTINFOMAXHEIGHT];

    init_color_xlat((unsigned char)fcolor, (unsigned char)bcolor);

    int orig_x = x;

    while (*str && y < vph) {
        unsigned char ch = (unsigned char)*str++;

        if (ch == '\n' || ch == '\r') {
            y += maxh + FontYSpacing;
            curline = base + view->Get_Offset() + y * pitch;
            draw = curline + orig_x;
            x = orig_x;
            continue;
        }

        int cw = (unsigned char)width_tbl[ch];
        if (x + cw > vpw) {
            y += maxh + FontYSpacing;
            curline = base + view->Get_Offset() + y * pitch;
            draw = curline + orig_x;
            x = orig_x;
            if (y >= vph)
                break;
        }

        unsigned short off = *((unsigned short *)off_tbl + ch);
        unsigned char top = height_tbl[ch * 2];
        unsigned char chh = height_tbl[ch * 2 + 1];
        unsigned char bot = maxh - (top + chh);
        unsigned char *data = font + off;

        unsigned char *dest = draw;
        for (int r = 0; r < top; ++r) {
            if (bcolor)
                memset(dest, bcolor, cw);
            dest += pitch;
        }

        int bytes_per = (cw + 1) / 2;
        for (int r = 0; r < chh; ++r) {
            unsigned char *dst_row = dest + r * pitch;
            unsigned char *src_row = data + r * bytes_per;
            for (int c = 0; c < cw; ++c) {
                unsigned char byte = src_row[c / 2];
                unsigned char nibble = (c & 1) ? (byte & 0x0F) : (byte >> 4);
                unsigned char col = Font_Color_Xlat[(c & 1) ? nibble : (nibble << 4)];
                if (col)
                    dst_row[c] = col;
                else if (bcolor)
                    dst_row[c] = bcolor;
            }
        }

        dest += chh * pitch;
        for (int r = 0; r < bot; ++r) {
            if (bcolor)
                memset(dest + r * pitch, bcolor, cw);
        }

        draw += cw + FontXSpacing;
        x += cw + FontXSpacing;
    }

    if (y >= vph)
        return 0;

    return (unsigned long)(draw - (base + view->Get_Offset()));
}

LONG Buffer_Print(void *thisptr, const char *str, int x, int y, int fcolor, int bcolor)
{
    /* linear surfaces handled the same way for the stub */
    return MCGA_Print(thisptr, str, x, y, fcolor, bcolor);
}

void *Get_Font_Palette_Ptr(void)
{
    init_color_xlat(0, 0);
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

