#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "udpOverTcp.h"
#include <string.h> /* memset */
#include <unistd.h> /* close */

struct sockaddr_in dest;
struct sockaddr_in src;
struct hostent* hp;

int UOTsocket()
{
    return socket(AF_INET, SOCK_DGRAM, 0);
}

/* Connect to specified address */

int UOTconnect(int sockfd, struct sockaddr *serv_addr, int addrlen)
{


    return 0;
}

int UOTsend(int sockfd, const void * msg, int len, int flags)
{

    return 0;
}

/* Bind custom TCP socket to specified address */
int UOTbind(int sockfd, struct sockaddr_in *my_addr, int addrlen)
{
    my_addr->sin_family = AF_INET;
    my_addr->sin_port = htons(PORT);
    my_addr->sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr *) my_addr, addrlen) < 0)
    {
        perror("bind");
        return -1;
    }
    return sockfd;
}

/* Receive message on specified socket */
int UOTrecv(int sockfd, void *buf, int len, struct sockaddr_in * cliAddr,  int size)
{
        fprintf(stderr, "in recive\n");
        size_t inbytes = len;
        struct iovec iovrecv[2];

        msgrecv.msg_name = NULL;
        msgrecv.msg_namelen = 0;
        msgrecv.msg_iov = iovrecv;
        msgrecv.msg_iovlen = 2;
        iovrecv[0].iov_base = &recvhdr;
        iovrecv[0].iov_len = sizeof (struct hdr);
        iovrecv[1].iov_base = buf;
        iovrecv[1].iov_len = inbytes;

        int n = 0;
        n = recvmsg(sockfd, &msgrecv, 0);
        fprintf(stderr, "recv %4d\n", recvhdr.seq);
}