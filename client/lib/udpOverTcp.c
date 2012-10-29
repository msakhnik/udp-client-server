#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "udpOverTcp.h"
#include <string.h> /* memset */
#include <unistd.h> /* close */
int UOTsocket()
{
    return socket(AF_INET, SOCK_DGRAM, 0);
}

/* Connect to specified address */

int UOTconnect(int sockfd, struct sockaddr *serv_addr, int addrlen)
{
    
    return 0;
}

int UOTsend (int sockfd, const void * msg, int len, int flags){
 
}

/* Receive message on specified socket */
int UOTrecv (int sockfd, void *buf, int len, int flags){
  
 
}