#include "miniaudio.h"  /* our wrapper */
#include "miniaudio/miniaudio.h"  /* external library */
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdatomic.h>

/* ---- Audio playback via miniaudio ---- */
static ma_device device;
static ra_audio_callback g_callback = NULL;

typedef struct {
    ra_timer_callback cb;
    unsigned int rate;
    atomic_int running;
    pthread_t thread;
} timer_event;

#define MAX_TIMER_EVENTS 8
static timer_event timers[MAX_TIMER_EVENTS];

static void data_callback(ma_device* dev, void* output, const void* input, ma_uint32 frame_count)
{
    (void)input;
    if (g_callback) {
        g_callback(output, frame_count);
    } else {
        memset(output, 0, frame_count * ma_get_bytes_per_frame(dev->playback.format, dev->playback.channels));
    }
}

int ra_audio_init(unsigned int sample_rate, unsigned int channels, ra_audio_callback cb)
{
    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format   = ma_format_s16;
    config.playback.channels = channels;
    config.sampleRate        = sample_rate;
    config.dataCallback      = data_callback;

    g_callback = cb;
    if (ma_device_init(NULL, &config, &device) != MA_SUCCESS) {
        return -1;
    }
    if (ma_device_start(&device) != MA_SUCCESS) {
        ma_device_uninit(&device);
        return -1;
    }
    return 0;
}

void ra_audio_shutdown(void)
{
    ma_device_uninit(&device);
    g_callback = NULL;
}

void ra_timer_init(void)
{
    memset(timers, 0, sizeof(timers));
}

void ra_timer_uninit(void)
{
    for (int i = 0; i < MAX_TIMER_EVENTS; ++i) {
        ra_timer_remove(i);
    }
}

static void* timer_thread(void* arg)
{
    timer_event* t = (timer_event*)arg;
    while (atomic_load(&t->running)) {
        if (t->cb) t->cb();
        usleep(1000000 / t->rate);
    }
    return NULL;
}

int ra_timer_register(unsigned int rate, ra_timer_callback cb, int* handle)
{
    if (!cb || rate == 0 || !handle) return -1;
    for (int i = 0; i < MAX_TIMER_EVENTS; ++i) {
        if (!atomic_load(&timers[i].running)) {
            timers[i].cb = cb;
            timers[i].rate = rate;
            atomic_store(&timers[i].running, 1);
            if (pthread_create(&timers[i].thread, NULL, timer_thread, &timers[i]) != 0) {
                atomic_store(&timers[i].running, 0);
                return -1;
            }
            *handle = i;
            return 0;
        }
    }
    return -1;
}

void ra_timer_remove(int handle)
{
    if (handle < 0 || handle >= MAX_TIMER_EVENTS) return;
    if (atomic_load(&timers[handle].running)) {
        atomic_store(&timers[handle].running, 0);
        pthread_join(timers[handle].thread, NULL);
    }
}

