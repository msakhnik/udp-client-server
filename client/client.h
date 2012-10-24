#ifndef CLIENT_H
#define	CLIENT_H

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
    
int CreateConnection(SAI*, int);
void SendMessage(FILE *, int, const SA *, size_t);
void CloseConnection(int);
#ifdef	__cplusplus
}
#endif

#endif	/* CLIENT_H */

