#include <stdint.h>
#include <string.h>
#include "debug_log.h"

static const int8_t bit2_table[4]  = { -2, -1, 0, 1 };
static const int8_t bit4_table[16] = { -9,-8,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,8 };

long Decompress_Frame(void *source, void *dest, long size)
{
    LOG_CALL("Decompress_Frame tool C version\n");
    uint8_t *s = (uint8_t *)source;
    uint8_t *d = (uint8_t *)dest;
    int previous = 0x80;
    long consumed = 0;

    while (size > 0) {
        uint8_t code = *s++;
        int count = (code & 0x3F) + 1;
        consumed++;

        switch (code >> 6) {
        case 0:
            if ((count - 1) & 0x20) {
                int delta = (count - 1) & 0x1F;
                if (delta & 0x10) delta |= 0xFFE0;
                previous += delta;
                if (previous < 0) previous = 0; else if (previous > 255) previous = 255;
                *d++ = (uint8_t)previous;
                size--;
            } else {
                if (count > size) count = (int)size;
                memcpy(d, s, count);
                consumed += count;
                d += count;
                s += count;
                size -= count;
                previous = d[-1];
            }
            break;
        case 1:
            while (count-- && size >= 2) {
                uint8_t delta = *s++;
                consumed--;
                previous += bit4_table[delta & 0x0F];
                if (previous < 0) previous = 0; else if (previous > 255) previous = 255;
                *d++ = (uint8_t)previous;
                size--;
                previous += bit4_table[(delta >> 4) & 0x0F];
                if (previous < 0) previous = 0; else if (previous > 255) previous = 255;
                *d++ = (uint8_t)previous;
                size--;
            }
            break;
        case 2:
            while (count-- && size >= 4) {
                uint8_t delta = *s++;
                consumed--;
                for (int i = 0; i < 4; ++i) {
                    previous += bit2_table[(delta >> (i*2)) & 3];
                    if (previous < 0) previous = 0; else if (previous > 255) previous = 255;
                    *d++ = (uint8_t)previous;
                    size--;
                }
            }
            break;
        default:
            if (count > size) count = (int)size;
            memset(d, (uint8_t)previous, count);
            d += count;
            size -= count;
            break;
        }
    }

    return consumed;
}
