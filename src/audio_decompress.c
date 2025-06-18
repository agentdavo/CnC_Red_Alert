#include "audio_decompress.h"
#include <string.h>
#include <stdint.h>

#if ENABLE_ASM
extern long Decompress_Frame(void *source, void *dest, long size);
extern void sosCODECInitStream(_SOS_COMPRESS_INFO *info);
extern unsigned long sosCODECDecompressData(_SOS_COMPRESS_INFO *info, unsigned long bytes);
extern unsigned long General_sosCODECDecompressData(_SOS_COMPRESS_INFO *info, unsigned long bytes);

long aud_decompress_frame(void *source, void *dest, long size)
{
    return Decompress_Frame(source, dest, size);
}

void aud_sos_init_stream(_SOS_COMPRESS_INFO *info)
{
    sosCODECInitStream(info);
}

unsigned long aud_sos_decompress(_SOS_COMPRESS_INFO *info, unsigned long bytes)
{
    return sosCODECDecompressData(info, bytes);
}

unsigned long aud_sos_decompress_general(_SOS_COMPRESS_INFO *info, unsigned long bytes)
{
    return General_sosCODECDecompressData(info, bytes);
}

#else /* ENABLE_ASM == OFF */

static const int8_t bit2_table[4]  = { -2, -1, 0, 1 };
static const int8_t bit4_table[16] = { -9,-8,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,8 };

long aud_decompress_frame(void *source, void *dest, long size)
{
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
                previous = (uint8_t)(previous + delta);
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

static const int index_table[16] = { -1,-1,-1,-1,2,4,6,8,-1,-1,-1,-1,2,4,6,8 };
static const int step_table[89] = {
    7,8,9,10,11,12,13,14,16,17,19,21,23,25,28,31,
    34,37,41,45,50,55,60,66,73,80,88,97,107,118,130,143,
    157,173,190,209,230,253,279,307,337,371,408,449,494,544,598,658,
    724,796,876,963,1060,1166,1282,1411,1552,1707,1878,2066,2272,2499,2749,3024,
    3327,3660,4026,4428,4871,5358,5894,6484,7132,7845,8630,9493,10442,11487,12635,13899,
    15289,16818,18500,20350,22385,24623,27086,29794,32767
};

static inline int decode_nibble(int nibble, int *predictor, int *index)
{
    int step = step_table[*index];
    int diff = step >> 3;
    if (nibble & 1) diff += step >> 2;
    if (nibble & 2) diff += step >> 1;
    if (nibble & 4) diff += step;
    if (nibble & 8) *predictor -= diff; else *predictor += diff;
    if (*predictor > 32767) *predictor = 32767;
    else if (*predictor < -32768) *predictor = -32768;
    *index += index_table[nibble & 0x0F];
    if (*index < 0) *index = 0; else if (*index > 88) *index = 88;
    return *predictor;
}

static unsigned long decode_adpcm_mono16(_SOS_COMPRESS_INFO *info, unsigned long bytes)
{
    const uint8_t *src = (const uint8_t *)info->lpSource;
    int16_t *dst = (int16_t *)info->lpDest;
    unsigned long samples = bytes / 2;
    int predictor = info->dwPredicted;
    int index = info->wIndex;

    for (unsigned long i = 0; i < samples; ++i) {
        int nibble = (i & 1) ? (src[i/2] >> 4) & 0xF : src[i/2] & 0xF;
        dst[i] = (int16_t)decode_nibble(nibble, &predictor, &index);
    }

    info->dwPredicted = predictor;
    info->wIndex = index;
    info->lpSource += (samples + 1) / 2;
    info->lpDest += bytes;
    info->dwSampleIndex += samples;
    return bytes;
}

void aud_sos_init_stream(_SOS_COMPRESS_INFO *info)
{
    info->wIndex = info->wIndex2 = 0;
    info->wStep = info->wStep2 = 7;
    info->dwPredicted = info->dwPredicted2 = 0;
}

unsigned long aud_sos_decompress(_SOS_COMPRESS_INFO *info, unsigned long bytes)
{
    if (info->wBitSize == 16 && info->wChannels == 1)
        return decode_adpcm_mono16(info, bytes);
    return 0;
}

unsigned long aud_sos_decompress_general(_SOS_COMPRESS_INFO *info, unsigned long bytes)
{
    return aud_sos_decompress(info, bytes);
}

#endif /* ENABLE_ASM */
