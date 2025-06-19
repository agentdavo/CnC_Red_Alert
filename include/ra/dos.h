#ifndef DOS_H
#define DOS_H
#include <stdint.h>

struct find_t {
    char reserved[21];
    char attrib;
    uint16_t time;
    uint16_t date;
    uint32_t size;
    char name[13];
};

static inline int _dos_findfirst(const char *path, unsigned attrib, struct find_t *res) { (void)path; (void)attrib; (void)res; return -1; }
static inline int _dos_findnext(struct find_t *res) { (void)res; return -1; }
static inline int _dos_open(const char *name, int mode, int *handle) { (void)name; (void)mode; if(handle) *handle=-1; return -1; }
static inline int _dos_close(int handle) { (void)handle; return -1; }
static inline int _dos_creat(const char *name, int attr, int *handle) { (void)name; (void)attr; if(handle) *handle=-1; return -1; }
static inline void _dos_getdrive(unsigned *drive) { if(drive) *drive = 0; }
static inline int _dos_getdiskfree(unsigned drive, void *info) { (void)drive; (void)info; return -1; }

#endif /* DOS_H */
