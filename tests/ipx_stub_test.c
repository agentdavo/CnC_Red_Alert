#include <assert.h>
#include <string.h>
#include "ipx.h"

/* Pointer exported by the stub */
extern int (*IPX_Get_Outstanding_Buffer95)(unsigned char *buffer);

int main(void) {
    IPX_SPX_Installed();
    assert(IPX_Open_Socket(0x4521) == 0);

    char send_buf[] = "ping";
    IPXHeaderType hdr = {0};
    hdr.DestNetworkNumber[0] = 127;
    hdr.DestNetworkNumber[1] = 0;
    hdr.DestNetworkNumber[2] = 0;
    hdr.DestNetworkNumber[3] = 1;
    hdr.DestNetworkSocket = 0x4521;
    ECBType ecb = {0};
    ecb.Packet[0].Address = &hdr;
    ecb.Packet[0].Length = sizeof(hdr);
    ecb.Packet[1].Address = send_buf;
    ecb.Packet[1].Length = sizeof(send_buf);
    IPX_Send_Packet(&ecb);
    assert(ecb.InUse == 0 && ecb.CompletionCode == 0);

    char recv_buf[16] = {0};
    ECBType rcv = {0};
    IPXHeaderType rcv_hdr = {0};
    rcv.Packet[0].Address = &rcv_hdr;
    rcv.Packet[0].Length = sizeof(rcv_hdr);
    rcv.Packet[1].Address = recv_buf;
    rcv.Packet[1].Length = sizeof(recv_buf);
    IPX_Listen_For_Packet(&rcv);
    assert(rcv.InUse == 0 && rcv.CompletionCode == 0);

    unsigned char buffer[600];
    assert(IPX_Get_Outstanding_Buffer95(buffer) == 0);

    IPX_Close_Socket(0x4521);
    return 0;
}
