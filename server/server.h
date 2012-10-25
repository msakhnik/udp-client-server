#ifndef SERVER_H
#define	SERVER_H

#ifdef	__cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define	SA struct sockaddr
#define	SAI struct sockaddr_in
#define	MAXLINE 1024

//structures for sending and reciving massages
static struct msghdr msgrecv, msgsend;

static struct hdr
{
    uint32_t seq;
    uint32_t ts;
} recvhdr, sendhdr;

struct iovec iovrecv[2], iovsend[2];
//end
int CreateConnection(SAI *, size_t);
void ReciveData(int);
void CloseConnection(int);
void RunEcho(int sock, SA *, size_t);
void _InitMsgRecv(char * message);

#ifdef	__cplusplus
}
#endif

#endif	/* SERVER_H */