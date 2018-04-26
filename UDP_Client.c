/*
    Simple udp client
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
 
#define SERVER "127.0.0.1"
#define BUFLEN 512  //Max length of buffer
#define PORT 6700   //The port on which to send data
 
void die(char *s)
{
    perror(s);
    exit(1);
}
 
int main(void)
{
    struct sockaddr_in si_svr;
    int sockfd, i, slen = sizeof(si_svr);
    char buf[BUFLEN];
    char message[BUFLEN];
 
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }
 
    memset((char *) &si_svr, 0, sizeof(si_svr));
    si_svr.sin_family = AF_INET;
    si_svr.sin_port = htons(PORT);
     
    if (inet_aton(SERVER, &si_svr.sin_addr) == 0) 
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
 
    while(1)
    {
        printf("Enter message : ");
        scanf("%s", message);
         
        //send the message
        if (sendto(sockfd, message, strlen(message), 0, (struct sockaddr *)&si_svr, slen)==-1)
        {
            die("sendto()");
        }
         
        //receive a reply and print it
        //clear the buffer by filling null, it might have previously received data
        memset(buf,'\0', BUFLEN);
        //try to receive some data, this is a blocking call
        if (recvfrom(sockfd, buf, BUFLEN, 0, (struct sockaddr *) &si_svr, &slen) == -1)
        {
            die("recvfrom()");
        }
     
        printf("%s", buf);    
    }
 
    close(sockfd);
    return 0;
}
