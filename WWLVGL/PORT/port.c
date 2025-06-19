#include "../INCLUDE/port.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#include <time.h>
#endif

int ww_get_drive_type(const char *path)
{
#ifdef _WIN32
    return GetDriveTypeA(path);
#else
    struct stat st;
    return (stat(path, &st) == 0) ? DRIVE_FIXED : DRIVE_NO_ROOT_DIR;
#endif
}

void ww_sleep(unsigned int ms)
{
#ifdef _WIN32
    Sleep(ms);
#else
    clock_t start = clock();
    while (((clock() - start) * 1000 / CLOCKS_PER_SEC) < (clock_t)ms) {
        /* busy wait */
    }
#endif
}

unsigned long ww_get_ticks(void)
{
#ifdef _WIN32
    return GetTickCount();
#else
    return (unsigned long)(clock() * 1000 / CLOCKS_PER_SEC);
#endif
}
