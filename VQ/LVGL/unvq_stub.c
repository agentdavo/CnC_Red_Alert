#include <string.h>
#include "unvq.h"

/* Simple placeholder implementations for assembly routines. */

void UnVQ_2x2(unsigned char *codebook, unsigned char *pointers,
              unsigned char *buffer, unsigned long blocksperrow,
              unsigned long numrows, unsigned long bufwidth)
{
    (void)codebook; (void)pointers; (void)blocksperrow;
    (void)numrows; (void)bufwidth;
    memset(buffer, 0, bufwidth * numrows * 2);
}

void UnVQ_2x3(unsigned char *codebook, unsigned char *pointers,
              unsigned char *buffer, unsigned long blocksperrow,
              unsigned long numrows, unsigned long bufwidth)
{
    (void)codebook; (void)pointers; (void)blocksperrow;
    (void)numrows; (void)bufwidth;
    memset(buffer, 0, bufwidth * numrows * 3);
}

void UnVQ_4x2(unsigned char *codebook, unsigned char *pointers,
              unsigned char *buffer, unsigned long blocksperrow,
              unsigned long numrows, unsigned long bufwidth)
{
    (void)codebook; (void)pointers; (void)blocksperrow;
    (void)numrows; (void)bufwidth;
    memset(buffer, 0, bufwidth * numrows * 2);
}

void UnVQ_4x4(unsigned char *codebook, unsigned char *pointers,
              unsigned char *buffer, unsigned long blocksperrow,
              unsigned long numrows, unsigned long bufwidth)
{
    (void)codebook; (void)pointers; (void)blocksperrow;
    (void)numrows; (void)bufwidth;
    memset(buffer, 0, bufwidth * numrows * 4);
}

void UnVQ_4x2_Woofer(unsigned char *codebook, unsigned char *pointers,
                     unsigned char *buffer, unsigned long blocksperrow,
                     unsigned long numrows, unsigned long bufwidth)
{
    UnVQ_4x2(codebook, pointers, buffer, blocksperrow, numrows, bufwidth);
}

void UnVQ_4x2_VESA320_32K(unsigned char *codebook, unsigned char *pointers,
                           unsigned char *palette, unsigned long grains_per_win,
                           unsigned long dummy1, unsigned long dummy2)
{
    (void)codebook; (void)pointers; (void)palette;
    (void)grains_per_win; (void)dummy1; (void)dummy2;
}

void UnVQ_4x2_Xmode(unsigned char *codebook, unsigned char *pointers,
                    unsigned char *buffer, unsigned long blocksperrow,
                    unsigned long numrows, unsigned long dummy)
{
    (void)dummy;
    UnVQ_4x2(codebook, pointers, buffer, blocksperrow, numrows, blocksperrow*4);
}

void UnVQ_4x2_XmodeCB(unsigned char *cbdummy, unsigned char *pointers,
                      unsigned char *buffer, unsigned long blocksperrow,
                      unsigned long numrows, unsigned long dummy)
{
    (void)cbdummy; (void)dummy;
    UnVQ_4x2(NULL, pointers, buffer, blocksperrow, numrows, blocksperrow*4);
}

void Upload_4x2CB(unsigned char *codebook, unsigned long numentries)
{
    (void)codebook; (void)numentries;
}

void XlatePointers(unsigned char *pointers, unsigned long numpointers)
{
    (void)pointers; (void)numpointers;
}
