#include "server.h"

int CreateConnection(SAI* ptr_addr, size_t size)
{

    int sock = 0;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("socket");
        exit(1);
    }

    ptr_addr->sin_family = AF_INET;
    ptr_addr->sin_port = htons(7);
    ptr_addr->sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sock, (SA *) ptr_addr, size) < 0)
    {
        perror("bind");
        return -1;
    }

    return sock;
}

void ReciveData(int sock)
{
    char buf[1024];
    int bytes_read;
    while (1)
    {
        bytes_read = recvfrom(sock, buf, 1024, 0, NULL, NULL);
        buf[bytes_read] = '\0';
        printf("%s", buf);
        printf("\n");
    }
}

void CloseConnection(int sock)
{
    printf("\nClose Socket\n");
    close(sock);
}

static struct msghdr msgrecv;

static struct hdr
{
    uint32_t seq;
    uint32_t ts;
} recvhdr;

void RunEcho(int sock, SA * pcliaddr, size_t size)
{
    int n;
    size_t len;
    char mesg[MAXLINE];
    memset(mesg, 0, MAXLINE);
    for (;;)
    {
        size_t inbytes = MAXLINE;
        struct iovec iovrecv[2];

        msgrecv.msg_name = NULL;
        msgrecv.msg_namelen = 0;
        msgrecv.msg_iov = iovrecv;
        msgrecv.msg_iovlen = 2;
        iovrecv[0].iov_base = &recvhdr;
        iovrecv[0].iov_len = sizeof (struct hdr);
        iovrecv[1].iov_base = mesg;
        iovrecv[1].iov_len = inbytes;

        n = recvmsg(sock, &msgrecv, 0);
        fprintf(stderr, "recv %4d\n", recvhdr.seq);
        fprintf(stderr, "recv %s\n", mesg);
        /*
                len  = size;
                n = recvfrom(sock, mesg, MAXLINE, 0, pcliaddr, &len);
                sendto(sock, mesg, n, 0, pcliaddr, len);
         */
        memset(mesg, 0, MAXLINE);
    }
}