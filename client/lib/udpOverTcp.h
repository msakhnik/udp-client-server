/* 
 * File:   udpOverTcp.h
 * Author: morwin
 *
 * Created on 28 жовтня 2012, 12:41
 */

#ifndef UDPOVERTCP_H
#define	UDPOVERTCP_H

#ifdef	__cplusplus
extern "C"
{
#endif

#define PORT 3425  
#define HOSTNAME "localhost"
    
static struct msghdr msgsend;

static struct hdr
{
    uint32_t seq;
    uint32_t ts;
} sendhdr;
    
int UOTsocket();
int UOTbind (int sockfd, struct sockaddr *my_addr, int addrlen);
int UOTaccept (int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int UOTconnect (int sockfd, struct sockaddr *serv_addr, int addrlen);
int UOTsend (int sockfd, const void *msg, int len, int flags);
int UOTrecv (int sockfd, void *buf, int len, int flags); 
int UOTclose (int sockfd);


#ifdef	__cplusplus
}
#endif

#endif	/* UDPOVERTCP_H */

