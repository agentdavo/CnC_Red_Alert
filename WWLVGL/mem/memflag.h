/*
 * Memory allocation flags for the Red Alert memory system.
 * last updated: 2024-06-22
 */

#ifndef MEMFLAG_H
#define MEMFLAG_H

#include <stddef.h>
#include <stdint.h>
#include "wwstd.h"

/* Memory allocation flags used by Alloc */
typedef enum {
    MEM_NORMAL = 0x0000,
    MEM_NEW    = 0x0001,
    MEM_CLEAR  = 0x0002,
    MEM_REAL   = 0x0004,
    MEM_TEMP   = 0x0008,
    MEM_LOCK   = 0x0010
} MemoryFlagType;

/* Prototypes for VMPAGEIN.ASM */
void Force_VM_Page_In(void *buffer, int length);

/* Prototypes for ALLOC.C */
void *Alloc(unsigned long bytes_to_alloc, MemoryFlagType flags);
void Free(const void *pointer);
void DPMI_Lock(const void *ptr, long size);
void DPMI_Unlock(const void *ptr, long size);
void *Resize_Alloc(void *original_ptr, unsigned long new_size_in_bytes);
long Ram_Free(MemoryFlagType flag);
long Heap_Size(MemoryFlagType flag);
long Total_Ram_Free(MemoryFlagType flag);

/* Prototypes for MEM_COPY.ASM */
void Mem_Copy(const void *source, void *dest, unsigned long bytes_to_copy);

extern void (*Memory_Error)(void);
extern void (*Memory_Error_Exit)(char *string);
extern unsigned long MinRam;
extern unsigned long MaxRam;

static inline void *Add_Long_To_Pointer(const void *ptr, long size)
{
    return (void *)((const char *)ptr + size);
}

#endif /* MEMFLAG_H */
