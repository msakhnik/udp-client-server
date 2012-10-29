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
    int it = 0;
    for (;;)
    {
        memset(mesg, 0x0, MAXLINE);
        cliLen = sizeof (cliAddr);
        n = _RecvFrom(sock, mesg, (struct sockaddr *) &cliAddr, cliLen);
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
        
        //DEBUG
        //some packet is long time no answer
/*
        it++;
        printf("i=%d\n", it);
        if (it == 2)
            sleep(2);
*/
        sendto(sock, mesg, n, 0, (struct sockaddr *) &cliAddr, cliLen);
        memset(mesg, 0, MAXLINE);
    }
}