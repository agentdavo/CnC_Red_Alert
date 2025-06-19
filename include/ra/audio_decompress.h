#ifndef AUDIO_DECOMPRESS_H
#define AUDIO_DECOMPRESS_H

#include <stdint.h>

#ifndef _SOS_COMPRESS_INFO
struct _SOS_COMPRESS_INFO;
#endif

#ifdef __cplusplus
extern "C" {
#endif

long aud_decompress_frame(void *source, void *dest, long size);
void aud_sos_init_stream(_SOS_COMPRESS_INFO *info);
unsigned long aud_sos_decompress(_SOS_COMPRESS_INFO *info, unsigned long bytes);
unsigned long aud_sos_decompress_general(_SOS_COMPRESS_INFO *info, unsigned long bytes);

#ifdef __cplusplus
}
#endif

#endif /* AUDIO_DECOMPRESS_H */
