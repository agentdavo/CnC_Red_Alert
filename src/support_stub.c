#include <string.h>
#include "debug_log.h"

/* Stubs for remaining assembly routines not yet ported */

int LCW_Comp(void const *source, void *dest, int length)
{
    LOG_CALL("LCW_Comp C stub\n");
    memcpy(dest, source, (size_t)length);
    return length;
}
