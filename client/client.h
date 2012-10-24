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

#define	SA struct sockaddr_in
    
int CreateConnection(struct sockaddr_in *, int);
void SendMessage(SA* ptr_addr, int, char *, int);
void CloseConnection(int);

#ifdef	__cplusplus
}
#endif

#endif	/* CLIENT_H */

