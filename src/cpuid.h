#ifndef CPUID_H
#define CPUID_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

bool Detect_MMX_Availability(void);

extern char CPUType;
extern char VendorID[13];

#ifdef __cplusplus
}
#endif

#endif /* CPUID_H */
