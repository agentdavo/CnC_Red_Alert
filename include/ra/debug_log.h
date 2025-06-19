#ifndef DEBUG_LOG_H
#define DEBUG_LOG_H

#ifdef ENABLE_LOGGING
#include "lvgl/src/misc/lv_log.h"
#define LOG_CALL(fmt, ...) LV_LOG_INFO(fmt, __VA_ARGS__)
/* Generic message helper used by new logging wrappers */
#define LOG_MSG(msg) LV_LOG_USER("%s", msg)
/* Map legacy debug output to the new logging system */
#define WWDebugString(str) LV_LOG(str)
#else
#define LOG_CALL(fmt, ...) do { } while (0)
#define LOG_MSG(msg) do { } while (0)
#endif

#endif /* DEBUG_LOG_H */
