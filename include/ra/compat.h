#ifndef COMPAT_H
#define COMPAT_H

#include <strings.h>

#ifndef stricmp
#define stricmp strcasecmp
#endif
#ifndef strnicmp
#define strnicmp strncasecmp
#endif

#ifndef _MAX_FNAME
#define _MAX_FNAME 256
#endif
#ifndef _MAX_EXT
#define _MAX_EXT 256
#endif
#ifndef _MAX_PATH
#define _MAX_PATH 260
#endif
#ifndef _MAX_DIR
#define _MAX_DIR 256
#endif
#ifndef _MAX_DRIVE
#define _MAX_DRIVE 3
#endif

#include <stdio.h>

#ifndef HAVE_ITOA
static inline char *itoa(int value, char *str, int base)
{
    if (!str) return NULL;
    if (base == 16)
        snprintf(str, 33, "%x", value);
    else if (base == 8)
        snprintf(str, 33, "%o", value);
    else
        snprintf(str, 33, "%d", value);
    return str;
}
#endif

#ifndef HAVE_LTOA
static inline char *ltoa(long value, char *str, int base)
{
    if (!str) return NULL;
    if (base == 16)
        snprintf(str, 33, "%lx", value);
    else if (base == 8)
        snprintf(str, 33, "%lo", value);
    else
        snprintf(str, 33, "%ld", value);
    return str;
}
#endif

#endif /* COMPAT_H */
