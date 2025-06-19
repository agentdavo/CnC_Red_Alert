#include "../INCLUDE/port.h"
#ifdef _WIN32
#include <windows.h>
int ww_get_drive_type(const char *path)
{
    return GetDriveTypeA(path);
}
#else
#include <sys/stat.h>
int ww_get_drive_type(const char *path)
{
    struct stat st;
    return (stat(path, &st) == 0) ? DRIVE_FIXED : DRIVE_NO_ROOT_DIR;
}
#endif
