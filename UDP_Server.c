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
 
void die(char *s)
{
    perror(s);
    exit(1);
}
 
int main(void)
{
    struct sockaddr_in si_svr, si_client;
    int sockfd, recv_len;
    int cli_len = sizeof(si_client);
    char buf[BUFLEN];
    char gateway[IP_LEN];
    char subnet[IP_LEN];

    printf("Gateway: ");
    scanf("%s", gateway);

    printf("Subnet : ");
    scanf("%s", subnet);

    DHCP_Packet pkt; 
    initSrvDHCP(&pkt, gateway, subnet);

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
     
    //keep listening for data
    while (1)
    {
        printf("Waiting for data...");
        fflush(stdout);
         
        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(sockfd, buf, BUFLEN, 0, (struct sockaddr *)&si_client, &cli_len)) == -1)
            die("recvfrom()");
         
        //print details of the client/peer and the data received
        printf("Received packet from %s:%d\n", inet_ntoa(si_client.sin_addr), ntohs(si_client.sin_port));
        printf("Data: %s\n" , buf);
         
        //now reply the client with the same data
        if (sendto(sockfd, buf, recv_len, 0, (struct sockaddr*) &si_client, cli_len) == -1)
            die("sendto()");
    }
 
    close(sockfd);
    return 0;
}
