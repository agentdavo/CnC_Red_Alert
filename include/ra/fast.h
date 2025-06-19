#ifndef FAST_H
#define FAST_H

#ifdef __cplusplus
extern "C" {
#endif

#define ASSUCCESS 0

typedef struct FastPort {
    int status;
} FastPort;

typedef FastPort* FastPortHandle;

FastPortHandle PortOpenGreenleafFast(int com, int baud, int parity, int wordlen, int stopbits);
int FastGetPortHardware(int com, int *irqnum, int *address);
int FastSetPortHardware(int com, int irq, int port);
void PortClose(FastPortHandle port);

#ifdef __cplusplus
}
#endif

#endif /* FAST_H */
