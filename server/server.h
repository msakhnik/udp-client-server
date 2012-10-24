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

#define	SA struct sockaddr_in
    
int CreateConnection(struct sockaddr_in *, int);
void ReciveData(int);
void CloseConnection(int);

#ifdef	__cplusplus
}
#endif

#endif	/* SERVER_H */

