/*
 * audio/soundint.h - minimal sound interrupt helpers
 * Last updated: 2025-06-24
 */
#ifndef SOUNDINT_H
#define SOUNDINT_H

#include "wwstd.h"
#include "sound.h"

/* compression types used by legacy audio formats */
typedef enum {
    SCOMP_NONE = 0,
    SCOMP_WESTWOOD = 1,
    SCOMP_SONARC = 33,
    SCOMP_SOS = 99
} SCompressType;

#define SONARC_MARGIN 32

typedef struct {
    int dummy;
} SampleTrackerType;

typedef struct {
    int DigiHandle;
    int ServiceSomething;
    long MagicNumber;
    void *UncompBuffer;
    long StreamBufferSize;
    short StreamBufferCount;
    SampleTrackerType SampleTracker[MAX_SFX];
    unsigned int SoundVolume;
    unsigned int ScoreVolume;
    int _int;
} LockedDataType;

extern LockedDataType LockedData;

void Init_Locked_Data(void);
long Simple_Copy(void **source, long *ssize, void **alternate, long *altsize,
                 void **dest, long size);
long Sample_Copy(SampleTrackerType *st, void **source, long *ssize,
                 void **alternate, long *altsize, void *dest, long size,
                 SCompressType scomp, void *trailer, short int *trailersize);
void maintenance_callback(void);
void DigiCallback(unsigned int driverhandle, unsigned int callsource,
                  unsigned int sampleid);
void HMI_TimerCallback(void);

#endif /* SOUNDINT_H */
