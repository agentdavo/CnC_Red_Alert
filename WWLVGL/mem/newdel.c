/*
 * mem/newdel.c - memory allocation wrappers
 * Last updated: 2025-06-19
 */
#include "wwmem.h"
#include "memflag.h"
#include <stddef.h>

void *ww_new(size_t size)
{
    return Alloc((unsigned long)size, MEM_NEW);
}

void *ww_new_array(size_t size)
{
    return Alloc((unsigned long)size, MEM_NEW);
}

void ww_delete(void *ptr)
{
    Free(ptr);
}

void ww_delete_array(void *ptr)
{
    Free(ptr);
}
