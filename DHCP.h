/*
    Author      : Justin Muskopf
    Course      : CSCE 3530.501
    Assignment  : Programming Assignment 4
    Description : DHCP Simulator
*/

#ifndef DCHP_H
#define DCHP_H

#define IP_LEN 16    //length of IP string
#define OFFER_NUM 3  //number of IP offers
#define TIMEOUT 3600 //lifetime of IP

#pragma pack(8)
typedef struct DHCP_Packet
{
    int  trans_id;
    int  ack;
    char yiaddr[OFFER_NUM][IP_LEN];
    int lifetime;
} DHCP_Packet;

//size of DHCP_Packet object
static const int PACKET_SIZE = sizeof(DHCP_Packet) + 1;

void initSrvDHCP(DHCP_Packet *, char *, char *);
void initCliDHCP(DHCP_Packet *);
void printPacket(DHCP_Packet, char *);
void acceptIP(char *);

#endif
