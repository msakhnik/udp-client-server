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

#define TCP_seq unsigned int

#define TCP_HEADER_SIZE (sizeof(TCP_header))
#define TCP_MSS 1000
#define TCP_SEGMENT_SIZE (sizeof(TCP_segment))

#define LOCAL_SEGMENT 1
#define REMOTE_SEGMENT 2
#define TIMER_SEGMENT 3
#define DEST_INFO 1
#define SRC_INFO 2

#define TROLL_PORT 15000
#define TCPD_PORT 29000
    
#define HOSTNAME "localhost"
    
typedef struct{
  u_short sport;
  u_short dport;
  TCP_seq seq;
  TCP_seq ack;
  u_short off_res_flags;
  u_short win;
  u_short sum;
  u_short urp;

}TCP_header;
    
/* TCP Segment structure */
typedef struct{
  TCP_header  hdr;
  char        data[TCP_MSS];
}TCP_segment;

/* TCP Buffer Node */
struct _TCPBUF_ {
  TCP_segment ts;
  int len;
  int txCount;
  double lastTxTime;
  struct _TCPBUF_* next;
};

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

