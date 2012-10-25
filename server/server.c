#include "server.h"

int CreateConnection(SAI* ptr_addr, size_t size)
{

    int sock = 0;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("socket");
        return -1;
    }
    ptr_addr->sin_family = AF_INET;
    ptr_addr->sin_addr.s_addr = htonl(INADDR_ANY);
    ptr_addr->sin_port = htons(1500);
    int rc;
    rc = bind(sock, (struct sockaddr *) ptr_addr, size);
    if (rc < 0)
    {
        printf("Cannot bind port number\n");
        return -1;
    }

    printf("Waiting for data on port UDP\n");

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

void _InitMsgSend(SA * pservaddr, char * sendline, int size)
{
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
}

void _InitMsgRecv(char * message)
{
    msgrecv.msg_name = NULL;
    msgrecv.msg_namelen = 0;
    msgrecv.msg_iov = iovrecv;
    msgrecv.msg_iovlen = 2;
    iovrecv[0].iov_base = &recvhdr;
    iovrecv[0].iov_len = sizeof (struct hdr);
    iovrecv[1].iov_base = message;
    iovrecv[1].iov_len = MAXLINE;
}

void RunEcho(int sock, SA * pcliaddr, size_t size)
{
    int n;
    size_t cliLen;
    struct sockaddr_in cliAddr;
    char mesg[MAXLINE];
    for (;;)
    {
        memset(mesg, 0x0, MAXLINE);
        cliLen = sizeof (cliAddr);
        n = recvfrom(sock, mesg, MAXLINE, 0,
                     (struct sockaddr *) &cliAddr, &cliLen);
        if (n < 0)
        {
            printf("cannot receive data \n");
            continue;
        }
        printf("From %s:UDP%u : %s \n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), mesg);
        //DEBUG
        //Time with reciv packeges
        /*
        sleep(1);
         */
        sendto(sock, mesg, n, 0, (struct sockaddr *) &cliAddr, cliLen);
        memset(mesg, 0, MAXLINE);
    }
}