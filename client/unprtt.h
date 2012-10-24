#ifndef	__unp_rtt_h
#define	__unp_rtt_h

#include	<sys/types.h>	/* basic system data types */
#include	<sys/socket.h>	/* basic socket definitions */
#include	<sys/time.h>	/* timeval{} for select() */
#include	<time.h>		/* timespec{} for pselect() */
#include	<netinet/in.h>	/* sockaddr_in{} and other Internet defns */
#include	<arpa/inet.h>	/* inet(3) functions */
#include	<errno.h>
#include	<fcntl.h>		/* for nonblocking */
#include	<netdb.h>
#include	<signal.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/stat.h>	/* for S_xxx file mode constants */
#include	<sys/uio.h>		/* for iovec{} and readv/writev */
#include	<unistd.h>
#include	<sys/wait.h>
#include	<sys/un.h>		/* for Unix domain sockets */

struct rtt_info {
  float		rtt_rtt;	/* most recent measured RTT, in seconds */
  float		rtt_srtt;	/* smoothed RTT estimator, in seconds */
  float		rtt_rttvar;	/* smoothed mean deviation, in seconds */
  float		rtt_rto;	/* current RTO to use, in seconds */
  int		rtt_nrexmt;	/* # times retransmitted: 0, 1, 2, ... */
  unsigned int	rtt_base;	/* # sec since 1/1/1970 at start */
};

#define	RTT_RXTMIN      2	/* min retransmit timeout value, in seconds */
#define	RTT_RXTMAX     60	/* max retransmit timeout value, in seconds */
#define	RTT_MAXNREXMT 	3	/* max # times to retransmit */

				/* function prototypes */
void	 rtt_debug(struct rtt_info *);
void	 rtt_init(struct rtt_info *);
void	 rtt_newpack(struct rtt_info *);
int		 rtt_start(struct rtt_info *);
void	 rtt_stop(struct rtt_info *, unsigned int);
int		 rtt_timeout(struct rtt_info *);
unsigned int rtt_ts(struct rtt_info *);

extern int	rtt_d_flag;	/* can be set to nonzero for addl info */

#endif	/* __unp_rtt_h */
