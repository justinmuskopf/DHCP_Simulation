#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "DHCP.h"

#define NUM_STORED 50

typedef enum {FALSE, TRUE} BOOL;

char Used_Ips[NUM_STORED][IP_LEN];

BOOL isValidIp(char *ip, char *gateway)
{
	if (!strcmp(ip, gateway))
		return FALSE;
		
	int i;
	for (i = 0; i < NUM_STORED; i++)
	{
		if (!strlen(Used_Ips[i]))
			break;
		
		if (!strcmp(Used_Ips[i], ip))
			return FALSE;
	}
	
	return TRUE;
}

//Initialize the first server DHCP packet
void initSrvDHCP(DHCP_Packet *pkt, char *gateway, char *subnet)
{
    srand(time(NULL));

    int i;
    for (i = 0; i < OFFER_NUM; i++)
    {
        strcpy(pkt -> ip_offers[i], generateRandomIP(gateway, subnet));
    }

    pkt -> ack = 0;
}

//Initialize the first client DHCP packet
void initCliDHCP(DHCP_Packet *pkt)
{
    srand(time(NULL));
    pkt -> trans_id = generateRandomID();
    pkt -> ack = 0;
    strcpy(pkt -> yiaddr, "0.0.0.0");
}

//Generate a random valid IP
char *generateRandomIP(char *gateway, char *subnet)
{
    //Get the values of each part
    int gateArr[4], subArr[4];
    sscanf(gateway, "%d.%d.%d.%d", &gateArr[0], &gateArr[1], &gateArr[2], &gateArr[3]);
    sscanf(subnet,  "%d.%d.%d.%d", &subArr[0],  &subArr[1],  &subArr[2],  &subArr[3]);

    //Find where the subnet stops
    int idx;
    for (idx = 0; idx < 4; idx++)
    {
        if (subArr[idx] != 255)
            break;
    }

    //Invalid subnet
    if (idx == 4)
    {
        printf("Invalid Subnet! All IPs occupied.\n");
        exit(0);
    }

    //Beginning and end of the range
    int begin = gateArr[idx] & subArr[idx];
    int end = (~subArr[idx] & 255) | begin;
	
    //Beginning IP, End IP, Random IP
    int randArr[4];

    int i;
    for (i = 0; i < idx; i++)
    {
        randArr[i] = gateArr[i];
    }    

	//Create random IP
    char *randIP = malloc(sizeof(char) * IP_LEN);
	
	//Generate random IPs and make sure not already used
    do
    {
		randArr[idx] = (rand() % (end - begin)) + begin;
		
        for (i = idx + 1; i < 4; i++)
        {
            randArr[i] = rand() % 255;
        }
		
		sprintf(randIP, "%d.%d.%d.%d", randArr[0], randArr[1], randArr[2], randArr[3]);
		
    } while (!isValidIp(randIP, gateway);    
    
    printf("Random IP: %s\n", randIP);

    return randIP;
}

//Generate a random transaction ID
int generateRandomID()
{
    //Random ID from 100-999
    return (rand() % 900) + 100;
}
