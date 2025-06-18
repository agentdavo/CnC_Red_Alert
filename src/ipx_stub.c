#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  include <winsock2.h>
#  include <ws2tcpip.h>
#  include <BaseTsd.h>
typedef SOCKET socket_t;
#  ifndef ssize_t
typedef SSIZE_T ssize_t;
#  endif
#  ifndef CLOSESOCKET
#    define CLOSESOCKET closesocket
#  endif
#else
#  include <unistd.h>
#  include <arpa/inet.h>
#  include <sys/socket.h>
typedef int socket_t;
#  ifndef INVALID_SOCKET
#    define INVALID_SOCKET (-1)
#  endif
#  ifndef CLOSESOCKET
#    define CLOSESOCKET close
#  endif
#endif
#include "ipx.h"



static socket_t udp_socket = INVALID_SOCKET;
static unsigned short bound_port = 0;
#ifdef _WIN32
static bool wsa_initialized = false;
static WSADATA wsa_data;
#endif

int IPX_SPX_Installed(void)
{
    return 1; /* always available */
}

int IPX_Open_Socket(unsigned short port)
{
#ifdef _WIN32
    if (!wsa_initialized) {
        if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
            return -1;
        wsa_initialized = true;
    }
#endif
    if (udp_socket != INVALID_SOCKET) {
        CLOSESOCKET(udp_socket);
        udp_socket = INVALID_SOCKET;
    }
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        CLOSESOCKET(fd);
        return -1;
    }

    udp_socket = fd;
    bound_port = port;
    return 0;
}

int IPX_Close_Socket(unsigned short port)
{
    (void)port;
    if (udp_socket != INVALID_SOCKET) {
        CLOSESOCKET(udp_socket);
        udp_socket = INVALID_SOCKET;
#ifdef _WIN32
        if (wsa_initialized) {
            WSACleanup();
            wsa_initialized = false;
        }
#endif
    }
    return 0;
}

int IPX_Get_Connection_Number(void)
{
    return 1; /* stub implementation */
}

int IPX_Get_1st_Connection_Num(char *username)
{
    (void)username;
    return 1;
}

int IPX_Get_Internet_Address(int connection_number,
                             unsigned char *network_number,
                             unsigned char *physical_node)
{
    (void)connection_number;
    if (network_number) {
        uint32_t addr = htonl(INADDR_LOOPBACK);
        memcpy(network_number, &addr, sizeof(addr));
    }
    if (physical_node) {
        memset(physical_node, 0, 6);
    }
    return 0;
}

int IPX_Get_User_ID(int connection_number, char *user_id)
{
    (void)connection_number;
    if (user_id) {
        strcpy(user_id, "local");
    }
    return 0;
}

int IPX_Listen_For_Packet(struct ECB *ecb_ptr)
{
    if (udp_socket == INVALID_SOCKET || !ecb_ptr) {
        return -1;
    }
    ecb_ptr->InUse = 1;
    struct sockaddr_in from;
    socklen_t fromlen = sizeof(from);
    ssize_t len = recvfrom(udp_socket,
                           (char *)ecb_ptr->Packet[1].Address,
                           ecb_ptr->Packet[1].Length,
                           0,
                           (struct sockaddr *)&from,
                           &fromlen);
    if (len < 0) {
        ecb_ptr->CompletionCode = 1;
        ecb_ptr->InUse = 0;
        return -1;
    }
    IPXHeaderType *hdr = (IPXHeaderType *)ecb_ptr->Packet[0].Address;
    hdr->Length = htons((unsigned short)len + sizeof(IPXHeaderType));
    hdr->SourceNetworkSocket = ntohs(bound_port);
    memcpy(hdr->SourceNetworkNumber, &from.sin_addr, 4);
    ecb_ptr->CompletionCode = 0;
    ecb_ptr->InUse = 0;
    return 0;
}

void IPX_Send_Packet(struct ECB *ecb_ptr)
{
    if (udp_socket == INVALID_SOCKET || !ecb_ptr) {
        return;
    }
    ecb_ptr->InUse = 1;
    IPXHeaderType *hdr = (IPXHeaderType *)ecb_ptr->Packet[0].Address;
    struct sockaddr_in to;
    memset(&to, 0, sizeof(to));
    to.sin_family = AF_INET;
    memcpy(&to.sin_addr, hdr->DestNetworkNumber, 4);
    to.sin_port = htons(hdr->DestNetworkSocket);
    sendto(udp_socket,
           (const char *)ecb_ptr->Packet[1].Address,
           ecb_ptr->Packet[1].Length,
           0,
           (struct sockaddr *)&to,
           sizeof(to));
    ecb_ptr->CompletionCode = 0;
    ecb_ptr->InUse = 0;
}

int IPX_Get_Local_Target(unsigned char *dest_network,
                         unsigned char *dest_node,
                         unsigned short dest_socket,
                         unsigned char *bridge_address)
{
    (void)dest_network;
    (void)dest_node;
    (void)dest_socket;
    if (bridge_address) {
        memset(bridge_address, 0, 6);
    }
    return 0;
}

int IPX_Cancel_Event(struct ECB *ecb_ptr)
{
    (void)ecb_ptr;
    return 0;
}

void Let_IPX_Breath(void)
{
    /* no-op for UDP */
}

/* ---- Windows 95 style helper wrappers ---------------------------------- */

static int stub_open_socket95(int socket)
{
    return IPX_Open_Socket((unsigned short)socket);
}

static void stub_close_socket95(int socket)
{
    IPX_Close_Socket((unsigned short)socket);
}

static int stub_get_connection_number95(void)
{
    return IPX_Get_Connection_Number();
}

static int stub_send_packet95(unsigned char *immed,
                      unsigned char *buf,
                      int buflen,
                      unsigned char *net,
                      unsigned char *node)
{
    (void)immed;
    IPXHeaderType hdr = {0};
    memcpy(hdr.DestNetworkNumber, net, 4);
    memcpy(hdr.DestNetworkNode, node, 6);
    hdr.DestNetworkSocket = bound_port;
    ECBType ecb = {0};
    ecb.Packet[0].Address = &hdr;
    ecb.Packet[0].Length = sizeof(hdr);
    ecb.Packet[1].Address = buf;
    ecb.Packet[1].Length = (unsigned short)buflen;
    IPX_Send_Packet(&ecb);
    return ecb.CompletionCode;
}

static int stub_broadcast_packet95(unsigned char *buf, int buflen)
{
    unsigned char net[4] = {0xff, 0xff, 0xff, 0xff};
    unsigned char node_addr[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    return stub_send_packet95(NULL, buf, buflen, net, node_addr);
}

static int stub_get_local_target95(unsigned char *dest_network,
                           unsigned char *dest_node,
                           unsigned short dest_socket,
                           unsigned char *bridge_address)
{
    return IPX_Get_Local_Target(dest_network, dest_node,
                                dest_socket, bridge_address);
}

static int stub_get_outstanding_buffer95(unsigned char *buffer)
{
    if (udp_socket == INVALID_SOCKET || !buffer)
        return 0;

    struct sockaddr_in from;
    socklen_t flen = sizeof(from);
#ifdef _WIN32
    u_long avail = 0;
    if (ioctlsocket(udp_socket, FIONREAD, &avail) != 0 || avail == 0)
        return 0;
    if (avail > 1024)
        avail = 1024;
    int len = recvfrom(udp_socket,
                       (char *)(buffer + sizeof(IPXHeaderType)),
                       (int)avail,
                       0,
                       (struct sockaddr *)&from,
                       &flen);
    if (len <= 0)
        return 0;
#else
    ssize_t len = recvfrom(udp_socket,
                           buffer + sizeof(IPXHeaderType),
                           1024,
                           MSG_DONTWAIT,
                           (struct sockaddr *)&from,
                           &flen);
    if (len <= 0)
        return 0;
#endif

    IPXHeaderType *hdr = (IPXHeaderType *)buffer;
    memset(hdr, 0, sizeof(*hdr));
    hdr->Length = htons((unsigned short)len + sizeof(IPXHeaderType));
    memcpy(hdr->SourceNetworkNumber, &from.sin_addr, 4);
    hdr->SourceNetworkSocket = ntohs(from.sin_port);
    return 1;
}

static int stub_start_listening95(void)
{
    /* nothing required for UDP */
    return 1;
}

static void stub_shut_down95(void)
{
    /* nothing required */
}

/* Exported function pointers expected by ipx95.h consumers */
int (*IPX_Open_Socket95)(int) = stub_open_socket95;
void (*IPX_Close_Socket95)(int) = stub_close_socket95;
int (*IPX_Get_Connection_Number95)(void) = stub_get_connection_number95;
int (*IPX_Send_Packet95)(unsigned char *, unsigned char *, int, unsigned char *, unsigned char *) = stub_send_packet95;
int (*IPX_Broadcast_Packet95)(unsigned char *, int) = stub_broadcast_packet95;
int (*IPX_Get_Local_Target95)(unsigned char *, unsigned char *, unsigned short, unsigned char *) = stub_get_local_target95;
int (*IPX_Get_Outstanding_Buffer95)(unsigned char *) = stub_get_outstanding_buffer95;
int (*IPX_Start_Listening95)(void) = stub_start_listening95;
void (*IPX_Shut_Down95)(void) = stub_shut_down95;


/* ---------------------------------------------------------------------- */
/* Replacement for the IPXPROT/IPXREAL real-mode stub accessors.          */
/* These assemblies expose the address and size of a small real-mode      */
/* handler routine.  The modern code no longer uses a real-mode handler,  */
/* but some callers still expect these helpers to exist.  Provide trivial */
/* stand-ins that return the address of a tiny dummy block so the old     */
/* allocation logic can proceed without touching assembly.                */

static const unsigned char rm_stub_code[] = { 0xCB }; /* RETF */

void *Get_RM_IPX_Address(void)
{
    return (void *)rm_stub_code;
}

long Get_RM_IPX_Size(void)
{
    return (long)sizeof(rm_stub_code);
}

