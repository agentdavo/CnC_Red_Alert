#ifndef RA_MINIAUDIO_H
#define RA_MINIAUDIO_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*ra_audio_callback)(void* output, unsigned int frame_count);
typedef void (*ra_timer_callback)(void);

int ra_audio_init(unsigned int sample_rate, unsigned int channels, ra_audio_callback cb);
void ra_audio_shutdown(void);

void ra_timer_init(void);
void ra_timer_uninit(void);

int ra_timer_register(unsigned int rate, ra_timer_callback cb, int* handle);
void ra_timer_remove(int handle);

#ifdef __cplusplus
}
#endif

#endif /* RA_MINIAUDIO_H */
