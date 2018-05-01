#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "DHCP.h"

//Size of array
#define NUM_STORED 50

//BOOL enum
typedef enum {FALSE, TRUE} BOOL;

//IPs that are in use
static char Used_Ips[NUM_STORED][IP_LEN];

//Prototypes
char *generateRandomIP(char *, char *);
int  generateRandomID();

//Size of packet
const size_t PACKET_SIZE = sizeof(DHCP_Packet);

//Is the address valid (unused)?
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
    //Seed RNG
    srand(time(NULL));

    //Fill the IP offer array
    int i;
    for (i = 0; i < OFFER_NUM; i++)
    {
        bzero(pkt -> yiaddr[i], IP_LEN);
        strcpy(pkt -> yiaddr[i], generateRandomIP(gateway, subnet));
        //inet_aton(generateRandomIP(gateway, subnet), &pkt -> yiaddr[i]);
    }
   
    pkt -> trans_id = 0;
    pkt -> ack = 0;
    pkt -> lifetime = 0;
}

//Initialize the first client DHCP packet
void initCliDHCP(DHCP_Packet *pkt)
{
    //Seed RNG, get Transaction ID, set ACK and yiaddr
    srand(time(NULL));
    pkt -> trans_id = generateRandomID();
    pkt -> ack = 0;
    pkt -> lifetime = 0;

    //inet_aton("0.0.1.1", &pkt -> yiaddr[0]);
    strcpy(pkt -> yiaddr[0], "0.0.0.0");
    bzero(pkt -> yiaddr[1], IP_LEN);
    bzero(pkt -> yiaddr[2], IP_LEN);
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
    int numAddr = end - begin;

    //Init the random address array
    int i;
    for (i = 0; i < idx; i++)
    {
        randArr[i] = gateArr[i];
    }    

    //Calculate # of available hosts
    for (i = idx + 1; i < 4; i++)
    {
        numAddr *= 255;
    }
    numAddr -= 2;

    printf("Available Hosts: %d\n", numAddr);

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
		
    } while (!isValidIp(randIP, gateway));    
    
    return randIP;
}

//Generate a random transaction ID
int generateRandomID()
{
    //Random ID from 100-999
    return (rand() % 900) + 100;
}

//Print packet information to screen
void printPacket(DHCP_Packet pkt, char *title)
{
    printf("---------------------------\n");
    printf("|%s\n", title);
    printf("---------------------------\n");
    if (pkt.trans_id)
        printf("|Transaction ID: %d\n", pkt.trans_id);
    if (pkt.ack)
        printf("|ACK: %d\n", pkt.ack);
    printf("|yiaddr:\n");
    
    int i;
    for (i = 0; i < OFFER_NUM; i++)
    {
        if (strlen(pkt.yiaddr[i]))
            printf("|    %s\n", pkt.yiaddr[i]);
    }
    if (pkt.lifetime)
        printf("|Lifetime: %d\n", pkt.lifetime);

    printf("---------------------------\n");

}
