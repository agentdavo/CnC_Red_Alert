#ifndef DEBUG_LOG_H
#define DEBUG_LOG_H

#ifdef ENABLE_LOGGING
#include <stdio.h>
#define LOG_CALL(fmt, ...) do { fprintf(stderr, fmt, __VA_ARGS__); } while (0)
#else
#define LOG_CALL(fmt, ...) do { } while (0)
#endif

#endif /* DEBUG_LOG_H */
