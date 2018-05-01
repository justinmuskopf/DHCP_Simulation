#ifndef DCHP_H
#define DCHP_H

#define IP_LEN 16
#define OFFER_NUM 3
#define TIMEOUT 3600

#include <arpa/inet.h>

#pragma pack(8)
typedef struct DHCP_Packet
{
    int  trans_id;
    int  ack;
    char yiaddr[OFFER_NUM][IP_LEN];
    int lifetime;
} DHCP_Packet;

void initSrvDHCP(DHCP_Packet *, char *, char *);
void initCliDHCP(DHCP_Packet *);
void printPacket(DHCP_Packet, char *);
char *packetToPayload(DHCP_Packet, char *);
DHCP_Packet payloadToPacket(char *);

#endif
