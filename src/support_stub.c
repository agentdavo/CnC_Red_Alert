#include <string.h>
#include <math.h>
#include "debug_log.h"
#include "drawbuff.h"
#include "gbuffer.h"

/* C fallbacks for SUPPORT.ASM and 2SUPPORT.ASM */

void Fat_Put_Pixel(long x, long y, long color, long size, void *page)
{
    LOG_CALL("Fat_Put_Pixel C stub\n");
    if (!page || size <= 0)
        return;
    Buffer_Fill_Rect(page, (int)x, (int)y, (int)(x + size), (int)(y + size), (unsigned char)color);
}

char *strtrim(char *buffer)
{
    LOG_CALL("strtrim C stub\n");
    if (!buffer)
        return NULL;
    /* skip leading spaces and tabs */
    char *start = buffer;
    while (*start == ' ' || *start == '\t')
        ++start;
    char *end = start + strlen(start);
    while (end > start && (end[-1] == ' ' || end[-1] == '\t'))
        --end;
    size_t len = end - start;
    memmove(buffer, start, len);
    buffer[len] = '\0';
    return buffer;
}

static int color_diff(int r1,int g1,int b1,int r2,int g2,int b2)
{
    int dr = r1 - r2;
    int dg = g1 - g2;
    int db = b1 - b2;
    return dr*dr + dg*dg + db*db;
}

void *Conquer_Build_Fading_Table(void *palette, void *dest, long color, long frac)
{
    LOG_CALL("Conquer_Build_Fading_Table C stub\n");
    if (!palette || !dest)
        return dest;
    if (frac > 255)
        frac = 255;
    unsigned char *pal = (unsigned char *)palette;
    unsigned char *out = (unsigned char *)dest;
    unsigned char target_r = pal[color * 3];
    unsigned char target_g = pal[color * 3 + 1];
    unsigned char target_b = pal[color * 3 + 2];

    out[0] = 0;
    for (int idx = 1; idx < 240; ++idx) {
        unsigned char r = pal[idx * 3];
        unsigned char g = pal[idx * 3 + 1];
        unsigned char b = pal[idx * 3 + 2];
        int ideal_r = r - ((r - target_r) * frac) / 128;
        int ideal_g = g - ((g - target_g) * frac) / 128;
        int ideal_b = b - ((b - target_b) * frac) / 128;
        int best = color;
        int bestv = 1<<30;
        for (int id = 240; id < 255; ++id) {
            int pr = pal[id * 3];
            int pg = pal[id * 3 + 1];
            int pb = pal[id * 3 + 2];
            int diff = color_diff(pr, pg, pb, ideal_r, ideal_g, ideal_b);
            if (diff < bestv) { bestv = diff; best = id; if (!diff) break; }
        }
        out[idx] = (unsigned char)best;
    }
    for (int i = 240; i < 256; ++i)
        out[i] = (unsigned char)i;
    return dest;
}

void Remove_From_List(void **list, long *index, long element)
{
    LOG_CALL("Remove_From_List C stub\n");
    if (!list || !index || *index <= 0)
        return;
    long count = *index;
    for (long i = 0; i < count; ++i) {
        if ((long)list[i] == element) {
            for (long j = i; j < count - 1; ++j)
                list[j] = list[j + 1];
            (*index)--;
            break;
        }
    }
}

long Get_EAX(void)
{
    LOG_CALL("Get_EAX C stub\n");
    return 0;
}

unsigned Square_Root(unsigned val)
{
    LOG_CALL("Square_Root C stub\n");
    return (unsigned)sqrt((double)val);
}

int LCW_Comp(void const *source, void *dest, int length)
{
    LOG_CALL("LCW_Comp C stub\n");
    memcpy(dest, source, (size_t)length);
    return length;
}
