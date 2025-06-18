#include <string.h>
#include "unvq.h"

/* Basic C implementations of the VQ frame decode helpers. These mimic
 * the behavior of the original assembly routines closely enough for the
 * software renderer. */

static void copy_block(const unsigned char *src, unsigned char *dst,
                       unsigned width, unsigned height, unsigned stride)
{
    for (unsigned y = 0; y < height; ++y) {
        memcpy(dst + y * stride, src + y * width, width);
    }
}

void UnVQ_2x2(unsigned char *codebook, unsigned char *pointers,
              unsigned char *buffer, unsigned long blocksperrow,
              unsigned long numrows, unsigned long bufwidth)
{
    for (unsigned long r = 0; r < numrows; ++r) {
        unsigned char *row = buffer + r * 2 * bufwidth;
        for (unsigned long c = 0; c < blocksperrow; ++c) {
            const unsigned char *blk = codebook + pointers[r * blocksperrow + c] * 4;
            row[c * 2 + 0] = blk[0];
            row[c * 2 + 1] = blk[1];
            row[bufwidth + c * 2 + 0] = blk[2];
            row[bufwidth + c * 2 + 1] = blk[3];
        }
    }
}

void UnVQ_2x3(unsigned char *codebook, unsigned char *pointers,
              unsigned char *buffer, unsigned long blocksperrow,
              unsigned long numrows, unsigned long bufwidth)
{
    for (unsigned long r = 0; r < numrows; ++r) {
        unsigned char *row = buffer + r * 3 * bufwidth;
        for (unsigned long c = 0; c < blocksperrow; ++c) {
            const unsigned char *blk = codebook + pointers[r * blocksperrow + c] * 6;
            memcpy(row + c * 2, blk, 2);
            memcpy(row + bufwidth + c * 2, blk + 2, 2);
            memcpy(row + bufwidth * 2 + c * 2, blk + 4, 2);
        }
    }
}

void UnVQ_4x2(unsigned char *codebook, unsigned char *pointers,
              unsigned char *buffer, unsigned long blocksperrow,
              unsigned long numrows, unsigned long bufwidth)
{
    for (unsigned long r = 0; r < numrows; ++r) {
        unsigned char *row = buffer + r * 2 * bufwidth;
        for (unsigned long c = 0; c < blocksperrow; ++c) {
            const unsigned char *blk = codebook + pointers[r * blocksperrow + c] * 8;
            copy_block(blk, row + c * 4, 4, 2, bufwidth);
        }
    }
}

void UnVQ_4x4(unsigned char *codebook, unsigned char *pointers,
              unsigned char *buffer, unsigned long blocksperrow,
              unsigned long numrows, unsigned long bufwidth)
{
    for (unsigned long r = 0; r < numrows; ++r) {
        unsigned char *row = buffer + r * 4 * bufwidth;
        for (unsigned long c = 0; c < blocksperrow; ++c) {
            const unsigned char *blk = codebook + pointers[r * blocksperrow + c] * 16;
            copy_block(blk, row + c * 4, 4, 4, bufwidth);
        }
    }
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
    UnVQ_4x2(codebook, pointers, buffer, blocksperrow, numrows,
             blocksperrow * 4);
}

void UnVQ_4x2_XmodeCB(unsigned char *cbdummy, unsigned char *pointers,
                      unsigned char *buffer, unsigned long blocksperrow,
                      unsigned long numrows, unsigned long dummy)
{
    (void)cbdummy; (void)dummy;
    UnVQ_4x2(NULL, pointers, buffer, blocksperrow, numrows,
             blocksperrow * 4);
}

void Upload_4x2CB(unsigned char *codebook, unsigned long numentries)
{
    (void)codebook; (void)numentries;
}

void XlatePointers(unsigned char *pointers, unsigned long numpointers)
{
    (void)pointers; (void)numpointers;
}
