#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cpuid.h>
#include "debug_log.h"

unsigned char Random(void)
{
    LOG_CALL("Random C replacement\n");
    return (unsigned char)(rand() & 0xFF);
}

int Get_Random_Mask(int maxval)
{
    LOG_CALL("Get_Random_Mask C replacement\n");
    if (maxval <= 0)
        return 1;
    unsigned mask = 1;
    while (mask <= (unsigned)maxval)
        mask <<= 1;
    return (int)(mask - 1);
}

void Shake_Screen(int shakes)
{
    LOG_CALL("Shake_Screen stub\n");
    (void)shakes;
}

long Reverse_Long(long number)
{
    LOG_CALL("Reverse_Long C replacement\n");
    uint32_t n = (uint32_t)number;
    n = ((n & 0x00FF00FFu) << 8) | ((n & 0xFF00FF00u) >> 8);
    return (long)((n >> 16) | (n << 16));
}

short Reverse_Short(short number)
{
    LOG_CALL("Reverse_Short C replacement\n");
    uint16_t n = (uint16_t)number;
    n = (uint16_t)((n >> 8) | (n << 8));
    return (short)n;
}

long Swap_Long(long number)
{
    LOG_CALL("Swap_Long C replacement\n");
    uint32_t n = (uint32_t)number;
    return (long)((n >> 16) | (n << 16));
}

long Calculate_CRC(void *buffer, long length)
{
    LOG_CALL("Calculate_CRC C replacement\n");
    uint8_t *ptr = (uint8_t *)buffer;
    uint32_t crc = 0;
    while (length >= 4) {
        uint32_t val;
        memcpy(&val, ptr, 4);
        crc = (crc << 1) | (crc >> 31);
        crc += val;
        ptr += 4;
        length -= 4;
    }
    if (length) {
        uint32_t val = 0;
        for (int i = 0; i < length; ++i) {
            val |= (uint32_t)ptr[i] << (i * 8);
        }
        crc = (crc << 1) | (crc >> 31);
        crc += val;
    }
    return (long)crc;
}

static unsigned cpu_family(void)
{
    unsigned eax, ebx, ecx, edx;
    if (!__get_cpuid(1, &eax, &ebx, &ecx, &edx))
        return 3; /* 386 */
    return (eax >> 8) & 0xF;
}

uint16_t Processor(void)
{
    LOG_CALL("Processor C replacement\n");
    unsigned fam = cpu_family();
    if (fam <= 3)
        return 0; /* 386 */
    if (fam == 4)
        return 1; /* 486 */
    return 2;      /* Pentium or newer */
}

uint16_t Operating_System(void)
{
    LOG_CALL("Operating_System stub\n");
    return 1; /* DOS */
}

unsigned long random(unsigned long mod)
{
    LOG_CALL("random C replacement\n");
    if (mod == 0)
        return 0;
    return (unsigned long)(rand() % mod);
}

int Clip_Rect(int *x, int *y, int *dw, int *dh, int width, int height)
{
    LOG_CALL("Clip_Rect C replacement\n");
    if (!x || !y || !dw || !dh)
        return -1;
    int clipped = 0;
    if (*dw <= 0 || *dh <= 0)
        return -1;
    if (*x < 0) {
        *dw += *x;
        *x = 0;
        clipped = 1;
    }
    if (*y < 0) {
        *dh += *y;
        *y = 0;
        clipped = 1;
    }
    if (*x + *dw > width) {
        *dw = width - *x;
        clipped = 1;
    }
    if (*y + *dh > height) {
        *dh = height - *y;
        clipped = 1;
    }
    if (*dw <= 0 || *dh <= 0)
        return -1;
    return clipped;
}

int Confine_Rect(int *x, int *y, int dw, int dh, int width, int height)
{
    LOG_CALL("Confine_Rect C replacement\n");
    if (!x || !y)
        return -1;
    int moved = 0;
    if (*x < 0) {
        *x = 0;
        moved = 1;
    } else if (*x + dw > width) {
        *x = width - dw;
        moved = 1;
    }
    if (*y < 0) {
        *y = 0;
        moved = 1;
    } else if (*y + dh > height) {
        *y = height - dh;
        moved = 1;
    }
    return moved;
}
