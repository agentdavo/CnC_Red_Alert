#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "_file.h"

WORD ibm_getdisk(void)
{
    /* Drive numbers are meaningless on modern systems. */
    return 0;
}

WORD ibm_setdisk(WORD drive)
{
    (void)drive;
    return 0;
}

WORD ibm_close(WORD handle)
{
    return close(handle);
}

WORD ibm_unlink(BYTE const *name)
{
    return unlink((const char *)name);
}

LONG ibm_lseek(WORD handle, LONG offset, WORD where)
{
    return lseek(handle, offset, where);
}

UWORD ibm_read(WORD handle, VOID *ptr, UWORD bytes)
{
    return (UWORD)read(handle, ptr, bytes);
}

UWORD ibm_write(WORD handle, VOID *ptr, UWORD bytes)
{
    return (UWORD)write(handle, ptr, bytes);
}

WORD ibm_open(BYTE const *name, UWORD mode, WORD attrib)
{
    (void)attrib;
    return open((const char *)name, mode, 0666);
}

WORD ibm_chdir(BYTE const *path)
{
    return chdir((const char *)path);
}

