#include "cpuid.h"
#include "debug_log.h"
#include <cpuid.h>
#include <string.h>

char CPUType = 0;
char VendorID[13] = "Not available";

bool Detect_MMX_Availability(void)
{
    LOG_CALL("Detect_MMX_Availability C stub\n");
    unsigned int eax, ebx, ecx, edx;
    unsigned int max_leaf = __get_cpuid_max(0, NULL);

    if (max_leaf == 0)
    {
        CPUType = 4; /* 486 or earlier */
        return false;
    }

    if (__get_cpuid(0, &eax, &ebx, &ecx, &edx))
    {
        char vendor[13];
        memcpy(vendor, &ebx, 4);
        memcpy(vendor + 4, &edx, 4);
        memcpy(vendor + 8, &ecx, 4);
        vendor[12] = '\0';
        strncpy(VendorID, vendor, sizeof(VendorID) - 1);
        VendorID[sizeof(VendorID) - 1] = '\0';
    }

    if (!__get_cpuid(1, &eax, &ebx, &ecx, &edx))
    {
        CPUType = 4;
        return false;
    }

    CPUType = (char)((eax >> 8) & 0xF);

    return (edx & bit_MMX) != 0;
}

