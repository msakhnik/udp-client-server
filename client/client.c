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

static struct msghdr msgsend;

static struct hdr
{
    uint32_t seq;
    uint32_t ts;
} sendhdr;

void SendMessage(FILE * fp, int sock, SA * pservaddr, size_t size)
{
    int n;
    char sendline[MAXLINE], recvline[MAXLINE + 1];
    while (fgets(sendline, MAXLINE, fp) != NULL)
    {
        struct iovec iovsend[2];
        //Fiil header
        sendhdr.seq++; //THis number of sequence
        printf("%d\n", sendhdr.seq);
        sendhdr.ts = 5; //This time-out now const value

        msgsend.msg_name = pservaddr;
        msgsend.msg_namelen = size;
        msgsend.msg_iov = iovsend;
        msgsend.msg_iovlen = 2;
        //This is top header of message
        iovsend[0].iov_base = &sendhdr;
        iovsend[0].iov_len = sizeof (struct hdr);
        //This is data
        iovsend[1].iov_base = sendline;
        iovsend[1].iov_len = strlen(sendline);

        sendmsg(sock, &msgsend, 0);
        fputs(recvline, stdout);
    }
}

void CloseConnection(int sock)
{
    printf("\nClose Socket\n");
    close(sock);
}