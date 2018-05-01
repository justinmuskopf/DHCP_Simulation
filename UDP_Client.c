/*
    Simple udp client
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "DHCP.h" 

#define SERVER "127.0.0.1"
#define PORT 6700   //The port on which to send data
 
static const int PACKET_SIZE = sizeof(DHCP_Packet) + 1;

void chooseRandomIP(DHCP_Packet *);

void die(char *s)
{
    perror(s);
    exit(1);
}
 
int main(void)
{
    struct sockaddr_in si_svr;
    int sockfd, i, slen = sizeof(si_svr);
    DHCP_Packet pkt; 
    char *payload = malloc(PACKET_SIZE);

    //Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        die("socket");
 
    //Zero memory of server reference
    memset((char *) &si_svr, 0, sizeof(si_svr));
    si_svr.sin_family = AF_INET;
    si_svr.sin_port = htons(PORT);
     
    //Convert hostname to address
    if (inet_aton(SERVER, &si_svr.sin_addr) == 0) 
        die("inet_aton()");
 
    //Initialize first packet
    initCliDHCP(&pkt);
         
    //Send the packet
    if (sendto(sockfd, (DHCP_Packet *)&pkt, PACKET_SIZE, 0, (struct sockaddr *)&si_svr, slen) == -1)
        die("sendto()");
         
    //Receive from server
    if (recvfrom(sockfd, (DHCP_Packet *)&pkt, PACKET_SIZE, 0, (struct sockaddr *) &si_svr, &slen) == -1)
        die("recvfrom()");
 
    printPacket(pkt, "OFFER");

    chooseRandomIP(&pkt);

    pkt.trans_id++;

    //Send the packet
    if (sendto(sockfd, (DHCP_Packet *)&pkt, PACKET_SIZE, 0, (struct sockaddr *)&si_svr, slen) == -1)
        die("sendto()");


    //Receive from server
    if (recvfrom(sockfd, (DHCP_Packet *)&pkt, PACKET_SIZE, 0, (struct sockaddr *) &si_svr, &slen) == -1)
        die("recvfrom()");
    printPacket(pkt, "ACK");

    close(sockfd);
    free(payload);

    return 0;
}

//Choose an offered IP at random
void chooseRandomIP(DHCP_Packet *pkt)
{
    int idx = rand() % OFFER_NUM;
    
    strcpy(pkt -> yiaddr[0], pkt -> yiaddr[idx]);
  
    int i;
    for (i = 0; i < OFFER_NUM; i++)
    {
        if (i == idx)
            continue;
        bzero(pkt -> yiaddr[i], IP_LEN);
    }
}
