#ifndef WW_PLATFORM_H
#define WW_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

enum {
    DRIVE_UNKNOWN = 0,
    DRIVE_NO_ROOT_DIR = 1,
    DRIVE_REMOVABLE = 2,
    DRIVE_FIXED = 3,
    DRIVE_REMOTE = 4,
    DRIVE_CDROM = 5,
    DRIVE_RAMDISK = 6
};

int ww_get_drive_type(const char *path);

#ifdef __cplusplus
}
#endif

#endif /* WW_PLATFORM_H */
