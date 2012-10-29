#ifndef SERVER_H
#define SERVER_H

#ifdef __cplusplus
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

#define SA struct sockaddr
#define SAI struct sockaddr_in
#define MAXLINE 1024

int CreateConnection(SAI *, size_t);
void CloseConnection(int);
void RunEcho(int, size_t);

#ifdef __cplusplus
}
#endif


#endif /* SERVER_H */