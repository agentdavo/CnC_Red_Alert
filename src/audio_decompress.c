#include <ra/audio_decompress.h>
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
                consumed++;
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
                consumed++;
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

extern unsigned short IndexTable[];
extern long DiffTable[];

static unsigned long decode_adpcm_mono16(_SOS_COMPRESS_INFO *info, unsigned long numbytes)
{
    unsigned long token;
    long sample;
    unsigned int fastindex;
    unsigned char *inbuff = (unsigned char *)info->lpSource;
    unsigned short *outbuff = (unsigned short *)info->lpDest;

    fastindex = (unsigned int)info->dwSampleIndex;
    sample = info->dwPredicted;

    if (!numbytes)
        goto SkipLoop;

    do {
        token = *inbuff++;
        fastindex += token & 0x0f;
        sample += DiffTable[fastindex];
        fastindex = IndexTable[fastindex];
        if (sample > 32767L)
            sample = 32767L;
        if (sample < -32768L)
            sample = -32768L;
        *outbuff++ = (unsigned short)sample;

        fastindex += token >> 4;
        sample += DiffTable[fastindex];
        fastindex = IndexTable[fastindex];
        if (sample > 32767L)
            sample = 32767L;
        if (sample < -32768L)
            sample = -32768L;
        *outbuff++ = (unsigned short)sample;
    } while(--numbytes);

SkipLoop:
    info->dwSampleIndex = (unsigned long)fastindex;
    info->dwPredicted = sample;
    info->lpSource = (char *)inbuff;
    info->lpDest = (char *)outbuff;
    return numbytes << 2;
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
