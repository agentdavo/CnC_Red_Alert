/*
 * audio/soundint.c - legacy interrupt helpers
 * Last updated: 2025-06-24
 */

/* Minimal C11 replacements for the legacy interrupt helpers. */

#include <string.h>
#include <stdint.h>
#include "soundint.h"

/* Simple memory copy helper used by the old audio code. */
long Simple_Copy(void **source, long *ssize, void **alternate, long *altsize,
                 void **dest, long size)
{
    long out = 0;
    if (*source && *ssize) {
        long s = size;
        if (*ssize < s) s = *ssize;
        memcpy(*dest, *source, s);
        *source = (uint8_t *)(*source) + s;
        *ssize -= s;
        out += s;
    }
    if (out < size && alternate && altsize && *alternate && *altsize) {
        long s = size - out;
        if (*altsize < s) s = *altsize;
        memcpy((uint8_t *)(*dest) + out, *alternate, s);
        *alternate = (uint8_t *)(*alternate) + s;
        *altsize -= s;
        out += s;
    }
    return out;
}

/* Straightforward memory copy for decompressed frames. */
long Sample_Copy(SampleTrackerType *st, void **source, long *ssize,
                 void **alternate, long *altsize, void *dest, long size,
                 SCompressType scomp, void *trailer, short int *trailersize)
{
    (void)st; (void)scomp; (void)trailer; (void)trailersize;
    return Simple_Copy(source, ssize, alternate, altsize, &dest, size);
}

void maintenance_callback(void) {}
void DigiCallback(unsigned int driverhandle, unsigned int callsource,
                  unsigned int sampleid) {
    (void)driverhandle; (void)callsource; (void)sampleid;
}
void HMI_TimerCallback(void) {}
