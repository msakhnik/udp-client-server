/*
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

static struct msghdr msgrecv, msgsend;

static struct hdr
{
    uint32_t seq;
    uint32_t ts;
} recvhdr, sendhdr;

void RunEcho(int sock, SA * pcliaddr, size_t size)
{
    int n;
    size_t len;
    char mesg[MAXLINE];
    memset(mesg, 0, MAXLINE);
    for (;;)
    {
        size_t inbytes = MAXLINE;
        struct iovec iovrecv[2], sendline[MAXLINE];

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
        struct iovec iovsend[2];
        //Fiil header
        sendhdr.seq = 12; 
        printf("%d\n", sendhdr.seq);
        sendhdr.ts = 5; //This time-out now const value

        msgsend.msg_name = &pcliaddr;
        msgsend.msg_namelen = size;
        msgsend.msg_iov = iovsend;
        msgsend.msg_iovlen = 2;
        //This is top header of message
        iovsend[0].iov_base = &sendhdr;
        iovsend[0].iov_len = sizeof (struct hdr);
        //This is data
        iovsend[1].iov_base = mesg;
        iovsend[1].iov_len = strlen(mesg);

        sendmsg(sock, &msgsend, 0);
        perror("send");
        
        
        memset(mesg, 0, MAXLINE);
    }
}
 */
#include "server.h"

static struct msghdr msgrecv, msgsend;

static struct hdr
{
    uint32_t seq;
    uint32_t ts;
} recvhdr, sendhdr;

int CreateConnection(SAI* ptr_addr, size_t size)
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("socket");
        exit(1);
    }
    ptr_addr->sin_family = AF_INET;
    ptr_addr->sin_addr.s_addr = htonl(INADDR_ANY);
    ptr_addr->sin_port = htons(1500);
    int rc;
    rc = bind(sock, (struct sockaddr *) ptr_addr, size);
    if (rc < 0)
    {
        perror("Cannot bind port number");
        exit(1);
    }
    printf("Waiting for data on port UDP\n");
    return sock;
}


void CloseConnection(int sock)
{
    printf("\nClose Socket\n");
    close(sock);
}

int _RecvFrom(int sock, char* line, SA * cliAddr, int size)
{
    return recvfrom(sock, line, MAXLINE, 0,
                     (struct sockaddr *) cliAddr, &size);
}


void RunEcho(int sock, size_t size)
{
    int n;
    size_t cliLen;
    SAI cliAddr;
    char mesg[MAXLINE];
    for (;;)
    {
        memset(mesg, 0x0, MAXLINE);
        cliLen = sizeof (cliAddr);
        n = _RecvFrom(sock, mesg, (struct sockaddr *) &cliAddr, cliLen);
/*
        n = recvfrom(sock, mesg, MAXLINE, 0,
                     (struct sockaddr *) &cliAddr, &cliLen);
*/
        if (n < 0)
        {
            perror("cannot receive data");
            continue;
        }
        printf("From %s:UDP%u : %s \n",
                   inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), mesg);
        //DEBUG
        //Time with reciv packeges
        /*
sleep(1);
*/
        sendto(sock, mesg, n, 0, (struct sockaddr *) &cliAddr, cliLen);
        memset(mesg, 0, MAXLINE);
    }
}