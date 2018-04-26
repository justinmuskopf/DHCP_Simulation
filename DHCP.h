#ifndef DCHP_H
#define DCHP_H

#define IP_LEN 16
#define OFFER_NUM 3

typedef struct DHCP_Packet
{
    int  trans_id;
    int  ack;
    char yiaddr[IP_LEN];
    char ip_offers[OFFER_NUM][IP_LEN];
} DHCP_Packet;

char *generateRandomIP(char *, char *);
int  generateRandomID();
void initSrvDHCP(DHCP_Packet *, char *, char *);
void initCliDHCP(DHCP_Packet *);

#endif
