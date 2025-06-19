#include <stdlib.h>
#include <string.h>
#include <ra/debug_log.h>
#include "memflag.h"

void Mem_Copy(void const *source, void *dest, unsigned long bytes_to_copy)
{
    LOG_CALL("Mem_Copy C stub\n");
    if (!source || !dest || bytes_to_copy == 0)
        return;
    memmove(dest, source, (size_t)bytes_to_copy);
}

unsigned long Largest_Mem_Block(void)
{
    LOG_CALL("Largest_Mem_Block C stub\n");
    size_t step = 1024 * 1024; /* 1MB */
    size_t max_test = 64 * 1024 * 1024; /* 64MB upper bound */
    size_t size = step;
    size_t last_good = 0;
    while (size <= max_test) {
        void *ptr = malloc(size);
        if (!ptr)
            break;
        free(ptr);
        last_good = size;
        size += step;
    }
    return (unsigned long)last_good;
}

void Force_VM_Page_In(void *buffer, int length)
{
    LOG_CALL("Force_VM_Page_In C stub\n");
    if (!buffer || length <= 0)
        return;
    volatile unsigned char *ptr = (volatile unsigned char *)buffer;
    int page = 4096;
    for (int i = 0; i < length; i += page) {
        volatile unsigned char tmp = ptr[i];
        (void)tmp;
    }
}
