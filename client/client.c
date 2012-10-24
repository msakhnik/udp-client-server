#include "client.h"

int CreateConnection(SAI* ptr_addr, int size)
{
    int sock = 0;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("socket");
        return -1;
    }
    ptr_addr->sin_family = AF_INET;
    ptr_addr->sin_port = htons(7);
    ptr_addr->sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (connect(sock, (struct sockaddr *) ptr_addr, size) < 0)
    {
        perror("perror");
        return -1;
    }

    return sock;
}

void SendReciveMessage(FILE * fp, int sock, const SA * pservaddr, size_t size)
{
    int n;
    char sendline[MAXLINE], recvline[MAXLINE + 1];
    while (fgets(sendline, MAXLINE, fp) != NULL)
    {
        sendto(sock, sendline, strlen(sendline), 0, pservaddr, size);
        n = recvfrom(sock, recvline, MAXLINE, 0, NULL, NULL);
        recvline[n] = 0;
        printf("+%d+\n", n);
        fputs(recvline, stdout);
    }
}

void dg_cli(FILE *fp, int sock, const SA *pservaddr, size_t servlen)
{
    ssize_t n;
    char sendline[MAXLINE], recvline[MAXLINE + 1];
    while (fgets(sendline, MAXLINE, fp) != NULL)
    {
        n = Dg_send_recv(sock, sendline, strlen(sendline), recvline, MAXLINE, pservaddr, servlen);
        recvline[n] = 0;
        printf("+%d+\n", n);
        fputs(recvline, stdout);
    }
}

void CloseConnection(int sock)
{
    printf("\nClose Socket\n");
    close(sock);
}

static struct rtt_info rttinfo;
static int rttinit = 0;
static struct msghdr msgsend, msgrecv; /* assumed init to 0 */

static struct hdr
{
    uint32_t seq; /* sequence # */
    uint32_t ts; /* timestamp when sent */
} sendhdr, recvhdr;

static void sig_alrm(int signo);
static sigjmp_buf jmpbuf;

ssize_t dg_send_recv(int fd, const void *outbuff, size_t outbytes,
             void *inbuff, size_t inbytes,
             const SA *destaddr, socklen_t destlen)
{
    ssize_t n;
    struct iovec iovsend[2], iovrecv[2];

    if (rttinit == 0)
    {
        rtt_init(&rttinfo); /* first time we're called */
        rttinit = 1;
        rtt_d_flag = 1;
    }

    sendhdr.seq++;
    msgsend.msg_name = destaddr;
    msgsend.msg_namelen = destlen;
    msgsend.msg_iov = iovsend;
    msgsend.msg_iovlen = 2;
    iovsend[0].iov_base = &sendhdr;
    iovsend[0].iov_len = sizeof (struct hdr);
    iovsend[1].iov_base = outbuff;
    iovsend[1].iov_len = outbytes;

    msgrecv.msg_name = NULL;
    msgrecv.msg_namelen = 0;
    msgrecv.msg_iov = iovrecv;
    msgrecv.msg_iovlen = 2;
    iovrecv[0].iov_base = &recvhdr;
    iovrecv[0].iov_len = sizeof (struct hdr);
    iovrecv[1].iov_base = inbuff;
    iovrecv[1].iov_len = inbytes;
    /* end dgsendrecv1 */

    /* include dgsendrecv2 */
    signal(SIGALRM, sig_alrm);
    rtt_newpack(&rttinfo); /* initialize for this packet */

sendagain:
#ifdef	RTT_DEBUG
    fprintf(stderr, "send %4d: ", sendhdr.seq);
#endif
    sendhdr.ts = rtt_ts(&rttinfo);
    sendmsg(fd, &msgsend, 0);

    alarm(rtt_start(&rttinfo)); /* calc timeout value & start timer */
#ifdef	RTT_DEBUG
    rtt_debug(&rttinfo);
#endif

    if (sigsetjmp(jmpbuf, 1) != 0)
    {
        if (rtt_timeout(&rttinfo) < 0)
        {
            perror("dg_send_recv: no response from server, giving up");
            rttinit = 0; /* reinit in case we're called again */
            errno = ETIMEDOUT;
            return (-1);
        }
#ifdef	RTT_DEBUG
        err_msg("dg_send_recv: timeout, retransmitting");
#endif
        goto sendagain;
    }

    do
    {
        n = recvmsg(fd, &msgrecv, 0);
#ifdef	RTT_DEBUG
        fprintf(stderr, "recv %4d\n", recvhdr.seq);
#endif
    }
    while (n < sizeof (struct hdr) || recvhdr.seq != sendhdr.seq);

    alarm(0); /* stop SIGALRM timer */
    /* 4calculate & store new RTT estimator values */
    rtt_stop(&rttinfo, rtt_ts(&rttinfo) - recvhdr.ts);

    return (n - sizeof (struct hdr)); /* return size of received datagram */
}

static void
sig_alrm(int signo)
{
    siglongjmp(jmpbuf, 1);
}

/* end dgsendrecv2 */

ssize_t Dg_send_recv(int fd, const void *outbuff, size_t outbytes,
             void *inbuff, size_t inbytes,
             const SA *destaddr, socklen_t destlen)
{
    ssize_t n;

    n = dg_send_recv(fd, outbuff, outbytes, inbuff, inbytes,
                     destaddr, destlen);
    if (n < 0)
        perror("dg_send_recv error");

    return (n);
}