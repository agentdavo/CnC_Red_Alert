#ifndef DEBUG_LOG_H
#define DEBUG_LOG_H

#ifdef ENABLE_LOGGING
#include <stdio.h>
#define LOG_CALL(fmt, ...) do { fprintf(stderr, fmt, __VA_ARGS__); } while (0)
/* Generic message helper used by new logging wrappers */
#define LOG_MSG(msg) LOG_CALL("%s\n", msg)
/* Map legacy debug output to the new logging system */
#define WWDebugString(str) LOG_CALL("%s", str)
#else
#define LOG_CALL(fmt, ...) do { } while (0)
#define LOG_MSG(msg) do { } while (0)
#endif

#endif /* DEBUG_LOG_H */
