/*
#ifndef CLIENT_H
#define CLIENT_H

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
#include <signal.h>
#include <errno.h>
#include<stdint.h>

#define SA struct sockaddr
#define SAI struct sockaddr_in
#define MAXLINE 1024

int CreateConnection(SAI*, int);
void SendMessage(FILE *, int, SA *, size_t);
void CloseConnection(int);

#ifdef __cplusplus
}
#endif

#endif
*/
/* CLIENT_H */

#ifndef CLIENT_H
#define CLIENT_H

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
#include <signal.h>
#include <errno.h>
#include<stdint.h>

#define SA struct sockaddr
#define SAI struct sockaddr_in
#define MAXLINE 1024

int CreateConnection(SAI*, int);
//Get server name and init remote server structure
void GetServerHost(SAI *, int, char**);

//Wait for answer from server
int _IsReadable(int, int *, int);

//Main function - parse array and send through _SendTo function
void SendMessage(int, SA *, size_t, int, char**);

//Send message
void _SendTo(int, char*,SA *, size_t, int);

void CloseConnection(int);

#ifdef __cplusplus
}
#endif

#endif /* CLIENT_H */