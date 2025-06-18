#include "../fast.h"
#include <stdlib.h>

FastPortHandle PortOpenGreenleafFast(int com, int baud, int parity, int wordlen, int stopbits) {
    (void)com; (void)baud; (void)parity; (void)wordlen; (void)stopbits;
    FastPortHandle port = (FastPortHandle)malloc(sizeof(FastPort));
    if (port) port->status = ASSUCCESS;
    return port;
}

int FastGetPortHardware(int com, int *irqnum, int *address) {
    (void)com;
    if (irqnum) *irqnum = 0;
    if (address) *address = 0;
    return ASSUCCESS;
}

int FastSetPortHardware(int com, int irq, int port) {
    (void)com; (void)irq; (void)port;
    return ASSUCCESS;
}

void PortClose(FastPortHandle port) {
    free(port);
}
