#include <stddef.h>
#include "huffman.h"

/* C implementation of the HuffDecompress and BuildHuffTree routines
 * originally implemented in assembly. These are simplified but keep
 * the same behavior for order-0 Huffman streams used by the VQA
 * player. */

static long build_tree(TreeNode *nodes)
{
    nodes[513].count = (unsigned long)-1;
    long next = HUFF_EOS + 1;

    while (1) {
        long first = 513;
        long last = 513;
        for (long i = 0; i < next; ++i) {
            if (nodes[i].count == 0)
                continue;
            if (nodes[i].count < nodes[first].count) {
                last = first;
                first = i;
            } else if (nodes[i].count < nodes[last].count) {
                last = i;
            }
        }
        if (last == 513)
            break;
        nodes[next].count = nodes[first].count + nodes[last].count;
        nodes[first].count = 0;
        nodes[last].count = 0;
        nodes[next].child0 = (unsigned short)(first << 3);
        nodes[next].child1 = (unsigned short)(last << 3);
        ++next;
    }

    return (next - 1) * 8;
}

long HuffDecompress(unsigned char *data, unsigned char *buffer,
                    long length, char *work)
{
    TreeNode *nodes = (TreeNode *)work;
    unsigned char *src = data;
    long i = 0;

    /* Decode the run-length encoded count table. */
    unsigned char start = src[0];
    unsigned char stop  = src[1];
    src += 2;

    while (i < 256) {
        if (start != i) {
            nodes[i].count = 0;
            i++;
            continue;
        }

        for (int v = start; v <= stop && i < 256; ++v) {
            nodes[i++].count = *src++;
        }

        start = src[0];
        if (start == 0) {
            /* Terminator reached */
            src += 1;
            break;
        }
        stop = src[1];
        src += 2;
    }

    nodes[HUFF_EOS].count = 1;

    long root = build_tree(nodes);

    unsigned char cur = *src++;
    unsigned int mask = 0x80;
    unsigned char *dst = buffer;
    while ((dst - buffer) < length) {
        long node = root;
        while (node > HUFF_EOS * 8) {
            if (cur & mask)
                node = nodes[node / 8].child1;
            else
                node = nodes[node / 8].child0;
            mask >>= 1;
            if (!mask) {
                cur = *src++;
                mask = 0x80;
            }
        }
        if (node == HUFF_EOS * 8)
            break;
        *dst++ = (unsigned char)(node / 8);
    }

    return dst - buffer;
}

long BuildHuffTree(TreeNode *nodes)
{
    return build_tree(nodes);
}
