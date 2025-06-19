/*
 * Portable sound driver using miniaudio
 */

#include <cstdlib>
#include <cstring>
#include <cstdint>
#include "audio.h"
#include "soundint.h"
#include "miniaudio/miniaudio.h"

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

typedef struct {
    int16_t *data;
    long samples;
    long pos;
    int volume;
    int active;
} ma_slot;

static ma_slot slots[MAX_SFX];
static int g_channels = 2;
static unsigned int g_rate = 22050;
static ma_device device;

static void mix_callback(void *output, unsigned int frame_count)
{
    int16_t *out = (int16_t *)output;
    memset(out, 0, frame_count * g_channels * sizeof(int16_t));
    for (int i = 0; i < MAX_SFX; ++i) {
        if (!slots[i].active) continue;
        for (unsigned int f = 0; f < frame_count; ++f) {
            if (slots[i].pos >= slots[i].samples) {
                slots[i].active = 0;
                break;
            }
            int16_t s = slots[i].data[slots[i].pos++] * slots[i].volume / 255;
            for (int c = 0; c < g_channels; ++c) {
                long val = out[f * g_channels + c] + s;
                if (val > 32767) val = 32767;
                if (val < -32768) val = -32768;
                out[f * g_channels + c] = (int16_t)val;
            }
        }
    }
}

static void free_slot(int id)
{
    if (id < 0 || id >= MAX_SFX) return;
    free(slots[id].data);
    memset(&slots[id], 0, sizeof(slots[id]));
}

BOOL Audio_Init(void *window, int bits_per_sample, BOOL stereo,
                int rate, int reverse_channels)
{
    (void)window; (void)bits_per_sample; (void)reverse_channels;
    g_channels = stereo ? 2 : 1;
    g_rate = rate;
    memset(slots, 0, sizeof(slots));

    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format   = ma_format_s16;
    config.playback.channels = g_channels;
    config.sampleRate        = rate;
    config.dataCallback      = [](ma_device* dev, void* output, const void* input, ma_uint32 frame_count) {
        (void)dev; (void)input;
        mix_callback(output, frame_count);
    };

    if (ma_device_init(NULL, &config, &device) != MA_SUCCESS)
        return FALSE;
    if (ma_device_start(&device) != MA_SUCCESS) {
        ma_device_uninit(&device);
        return FALSE;
    }
    return TRUE;
}

void Sound_End(void)
{
    ma_device_uninit(&device);
    for (int i = 0; i < MAX_SFX; ++i) free_slot(i);
}

int Get_Free_Sample_Handle(int priority)
{
    (void)priority;
    for (int i = 0; i < MAX_SFX; ++i)
        if (!slots[i].active && !slots[i].data)
            return i;
    return -1;
}

static int load_sample_data(const void *sample, int id, int volume)
{
    AUDHeaderType hdr;
    memcpy(&hdr, sample, sizeof(hdr));
    if (hdr.Compression != SCOMP_NONE) return -1; /* unsupported */

    int bytes_per_sample = (hdr.Flags & AUD_FLAG_16BIT) ? 2 : 1;
    int channels = (hdr.Flags & AUD_FLAG_STEREO) ? 2 : 1;
    long frames = hdr.Size / (bytes_per_sample * channels);
    int16_t *buf = (int16_t *)malloc(frames * channels * sizeof(int16_t));
    if (!buf) return -1;

    const uint8_t *src = (const uint8_t *)sample + sizeof(hdr);
    if (bytes_per_sample == 1) {
        for (long i = 0; i < frames * channels; ++i)
            buf[i] = ((int16_t)src[i] - 128) << 8;
    } else {
        memcpy(buf, src, frames * channels * sizeof(int16_t));
    }

    free_slot(id);
    slots[id].data = buf;
    slots[id].samples = frames * channels;
    slots[id].pos = 0;
    slots[id].volume = volume;
    slots[id].active = 1;
    return id;
}

int Play_Sample_Handle(void const *sample, int priority, int volume,
                       signed short panloc, int id)
{
    (void)priority; (void)panloc;
    if (id < 0 || id >= MAX_SFX) return -1;
    return load_sample_data(sample, id, volume);
}

int Play_Sample(void const *sample, int priority, int volume, signed short panloc)
{
    int id = Get_Free_Sample_Handle(priority);
    if (id < 0) return -1;
    return Play_Sample_Handle(sample, priority, volume, panloc, id);
}

void Stop_Sample(int handle)
{
    free_slot(handle);
}

BOOL Sample_Status(int handle)
{
    return (handle >= 0 && handle < MAX_SFX && slots[handle].active) ? TRUE : FALSE;
}

BOOL Is_Sample_Playing(void const *sample)
{
    (void)sample; return FALSE;
}

void Stop_Sample_Playing(void const *sample)
{
    (void)sample;
}

int Set_Sound_Vol(int volume)
{
    for (int i = 0; i < MAX_SFX; ++i)
        if (slots[i].active) slots[i].volume = volume;
    return volume;
}

int Set_Score_Vol(int volume)
{
    return Set_Sound_Vol(volume);
}

void Fade_Sample(int handle, int ticks)
{
    (void)handle; (void)ticks;
}

int Get_Digi_Handle(void) { return 0; }
long Sample_Length(void const *sample)
{
    AUDHeaderType hdr; memcpy(&hdr, sample, sizeof(hdr));
    int bytes = hdr.Size;
    int bps = (hdr.Flags & AUD_FLAG_16BIT) ? 2 : 1;
    int ch = (hdr.Flags & AUD_FLAG_STEREO) ? 2 : 1;
    return bytes / (bps * ch);
}

BOOL Set_Primary_Buffer_Format(void) { return TRUE; }
BOOL Start_Primary_Sound_Buffer(BOOL forced) { (void)forced; return TRUE; }
void Stop_Primary_Sound_Buffer(void) {}
void Restore_Sound_Buffers(void) {}
void Sound_Callback(void) {}
void maintenance_callback(void) {}

