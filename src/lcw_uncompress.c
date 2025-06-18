#include <stdint.h>

/* Portable C implementation of the LCW_Uncompress routine. */
unsigned long LCW_Uncompress(void *source, void *dest, unsigned long length)
{
    (void)length; /* parameter ignored in original implementation */

    unsigned char *src = (unsigned char *)source;
    unsigned char *dst = (unsigned char *)dest;
    while (1) {
        unsigned char op = *src++;
        if (!(op & 0x80)) {
            unsigned int count = (op >> 4) + 3;
            unsigned char *copy = dst - ((unsigned int)*src++ + ((op & 0x0F) << 8));
            while (count--)
                *dst++ = *copy++;
        } else if (!(op & 0x40)) {
            if (op == 0x80)
                return (unsigned long)(dst - (unsigned char *)dest);
            unsigned int count = op & 0x3F;
            while (count--)
                *dst++ = *src++;
        } else if (op == 0xFE) {
            unsigned int count = src[0] + ((unsigned int)src[1] << 8);
            unsigned char data = src[2];
            uint32_t word = ((uint32_t)data << 24) | ((uint32_t)data << 16) |
                             ((uint32_t)data << 8) | data;
            src += 3;
            unsigned char *align = dst + 4 - ((uintptr_t)dst & 3);
            count -= (unsigned int)(align - dst);
            while (dst < align)
                *dst++ = data;
            uint32_t *wdst = (uint32_t *)dst;
            dst += (count & ~3u);
            while (wdst < (uint32_t *)dst) {
                *wdst++ = word;
                *wdst++ = word;
            }
            align = dst + (count & 3);
            while (dst < align)
                *dst++ = data;
        } else {
            unsigned int count;
            unsigned char *copy;
            if (op == 0xFF) {
                count = src[0] + ((unsigned int)src[1] << 8);
                copy = (unsigned char *)dest + src[2] + ((unsigned int)src[3] << 8);
                src += 4;
            } else {
                count = (op & 0x3F) + 3;
                copy = (unsigned char *)dest + src[0] + ((unsigned int)src[1] << 8);
                src += 2;
            }
            while (count--)
                *dst++ = *copy++;
        }
    }
}

