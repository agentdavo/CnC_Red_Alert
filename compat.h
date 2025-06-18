#ifndef COMPAT_H
#define COMPAT_H

#include <strings.h>

#ifndef stricmp
#define stricmp strcasecmp
#endif
#ifndef strnicmp
#define strnicmp strncasecmp
#endif

#endif /* COMPAT_H */
