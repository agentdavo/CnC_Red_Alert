#include <string.h>
#include "debug_log.h"
#include "lcw.h"

/* C implementation of the LCW_Comp routine originally written in assembly. */
int LCW_Comp(void const *source, void *dest, int length)
{
    const unsigned char *src = (const unsigned char *)source;
    const unsigned char *src_start = src;
    const unsigned char *src_end = src + length;
    unsigned char *dst = (unsigned char *)dest;
    unsigned char *dst_start = dst;

    if (length <= 0)
        return 0;

    /* first byte is encoded as a literal run of one */
    unsigned char *len_ptr = dst;
    *dst++ = 0x81; /* count = 1 */
    *dst++ = *src++;
    int in_len = 1;

    while (src < src_end) {
        /* check for long run of a single byte (>=65 repetitions) */
        int remaining = src_end - src;
        if (remaining >= 65 && src[0] == src[64]) {
            int run = 65;
            while (run < remaining && src[run] == src[0])
                run++;
            *dst++ = 0xFE;
            dst[0] = (unsigned char)run;
            dst[1] = (unsigned char)(run >> 8);
            dst[2] = src[0];
            dst += 3;
            src += run;
            in_len = 0;
            continue;
        }

        /* search for best previous match */
        int best_len = 0;
        int best_off = 0;
        for (const unsigned char *search = src_start; search < src; ++search) {
            int off = src - search;
            int max_len = src_end - src;
            int l = 0;
            while (l < max_len && search[l] == src[l])
                l++;
            if (l > best_len) {
                best_len = l;
                best_off = search - src_start;
                if (l == max_len)
                    break;
            }
        }

        if (best_len >= 3) {
            int offset = (int)(src - src_start) - best_off;
            if (best_len <= 10 && offset <= 0x0FFF) {
                /* short run */
                unsigned char high = ((best_len - 3) << 4) | ((offset >> 8) & 0x0F);
                *dst++ = high;
                *dst++ = (unsigned char)(offset & 0xFF);
            } else if (best_len <= 64) {
                /* medium run */
                *dst++ = (unsigned char)((best_len - 3) | 0xC0);
                dst[0] = (unsigned char)(offset & 0xFF);
                dst[1] = (unsigned char)((offset >> 8) & 0xFF);
                dst += 2;
            } else {
                if (best_len > 0xFFFF)
                    best_len = 0xFFFF;
                *dst++ = 0xFF;
                dst[0] = (unsigned char)(best_len & 0xFF);
                dst[1] = (unsigned char)((best_len >> 8) & 0xFF);
                dst[2] = (unsigned char)(offset & 0xFF);
                dst[3] = (unsigned char)((offset >> 8) & 0xFF);
                dst += 4;
            }
            src += best_len;
            in_len = 0;
            continue;
        }

        /* emit literal byte */
        if (!in_len || *len_ptr == 0xBF) {
            len_ptr = dst;
            *dst++ = 0x80;
            in_len = 1;
        }
        (*len_ptr)++;
        *dst++ = *src++;
    }

    *dst++ = 0x80; /* end code */
    return (int)(dst - dst_start);
}
