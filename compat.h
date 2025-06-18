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

#endif /* COMPAT_H */
