/*
    Simple udp server
*/
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h> 
#include "DHCP.h"

#define BUFLEN 512  //Max length of buffer
#define PORT 6700   //The port on which to listen for incoming data
 
typedef enum {FALSE, TRUE} BOOL;

static const int PACKET_SIZE = sizeof(DHCP_Packet) + 1;

//Error and die
void die(char *s)
{
    perror(s);
    exit(1);
}

//Simple address format check
BOOL isValidAddress(char *addr)
{
    int checkArr[4];
    
    int i;
    for (i = 0; i < 4; i++)
    {
        checkArr[i] = -1;
    }

    sscanf(addr, "%d.%d.%d.%d", &checkArr[0], &checkArr[1], &checkArr[2], &checkArr[3]);

    for (i = 0; i < 4; i++)
    {
        if (checkArr[i] == -1)
            return FALSE;
    }

    return TRUE;
}
 



int main(void)
{
    struct sockaddr_in si_svr, si_client;
    int sockfd, recv_len;
    int cli_len = sizeof(si_client);
    char gateway[IP_LEN];
    char subnet[IP_LEN];
    char *payload = malloc(PACKET_SIZE);
    DHCP_Packet pkt; 

    //Get valid gateway
    do
    {
        printf("Gateway (e.g. 192.168.1.1): ");
        scanf("%s", gateway);
    } while (!isValidAddress(gateway));

    //Get valid subnet
    do
    {
        printf("Subnet (e.g. 255.255.255.0): ");
        scanf("%s", subnet);
    } while (!isValidAddress(subnet));


    //create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        die("socket");
     
    // zero out the structure
    memset((char *) &si_svr, 0, sizeof(si_svr));
     
    si_svr.sin_family = AF_INET;
    si_svr.sin_port = htons(PORT);
    si_svr.sin_addr.s_addr = htonl(INADDR_ANY);
     
    //bind socket to port
    if (bind(sockfd, (struct sockaddr*)&si_svr, sizeof(si_svr)) == -1)
        die("bind");

    while (1)
    {
     
        printf("Waiting for client...\n");
         
        //Receive Discover Packet
        if ((recv_len = recvfrom(sockfd, (DHCP_Packet *)&pkt, PACKET_SIZE, 0, (struct sockaddr *)&si_client, &cli_len)) == -1)
            die("recvfrom()");
        printPacket(pkt, "DISCOVER");
    
        DHCP_Packet send_pkt;
        initSrvDHCP(&send_pkt, gateway, subnet);
        send_pkt.trans_id = pkt.trans_id;
        send_pkt.lifetime = TIMEOUT;   

        //Send Offer Packet
        if (sendto(sockfd, (DHCP_Packet *)&send_pkt, PACKET_SIZE, 0, (struct sockaddr*) &si_client, cli_len) == -1)
            die("sendto()");

        //Receive request packet
        if ((recv_len = recvfrom(sockfd, (DHCP_Packet *)&pkt, PACKET_SIZE, 0, (struct sockaddr *)&si_client, &cli_len)) == -1)
            die("recvfrom()");
        printPacket(pkt, "REQUEST");

        
        //ACK response
        pkt.ack = 1;
        if (sendto(sockfd, (DHCP_Packet *)&pkt, PACKET_SIZE, 0, (struct sockaddr*) &si_client, cli_len) == -1)
            die("sendto()");
    }     

    close(sockfd);
    
    free(payload);

    return 0;
}
