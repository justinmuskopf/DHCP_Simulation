/*
    Author      : Justin Muskopf
    Course      : CSCE 3530.501
    Assignment  : Programming Assignment 4
    Description : DHCP Simulator
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include "DHCP.h" 

#define SERVER "cse01"

//Chooses random IP 
void chooseRandomIP(DHCP_Packet *);

//Error and die
void die(char *s)
{
    perror(s);
    exit(1);
}
 
int main(int argc, char *argv[])
{
    struct sockaddr_in si_svr;
    struct hostent *host;
    int sockfd, i, slen = sizeof(si_svr);
    DHCP_Packet pkt; 
    int port;

    //make sure port is given
    if (argc != 2)
    {
        printf("Usage: %s [PORT NUMBER]\n", argv[0]);
        exit(1);
    }
    port = atoi(argv[1]);

    //Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        die("socket");
     
    if ((host = gethostbyname(SERVER)) == NULL)
        die("gethostbyname()");

    //Zero memory of server reference
    memset((char *) &si_svr, 0, sizeof(si_svr));
    si_svr.sin_family = AF_INET;
    si_svr.sin_port = htons(port);
    memcpy(&si_svr.sin_addr, host -> h_addr, host -> h_length);
 
    //Initialize first packet
    initCliDHCP(&pkt);
         
    //Send the packet
    if (sendto(sockfd, (DHCP_Packet *)&pkt, PACKET_SIZE, 0, (struct sockaddr *)&si_svr, slen) == -1)
        die("sendto()");
         
    //Receive from server
    if (recvfrom(sockfd, (DHCP_Packet *)&pkt, PACKET_SIZE, 0, (struct sockaddr *) &si_svr, &slen) == -1)
        die("recvfrom()");
 
    printPacket(pkt, "OFFER");

    //Choose randomly from offered IPs
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
