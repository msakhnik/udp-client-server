#include "server.h"

int CreateConnection(SAI* ptr_addr, size_t size)
{
    
    int sock = 0;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }
    
    ptr_addr->sin_family = AF_INET;
    ptr_addr->sin_port = htons(7);
    ptr_addr->sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sock,  (SA *)ptr_addr, size) < 0)
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
    while(1)
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

void RunEcho(int sock, SA * pcliaddr, size_t size)
{
    int n;
    size_t len;
    char  mesg[MAXLINE];
    while (1)
    {
        len  = size;
        n = recvfrom(sock, mesg, MAXLINE, 0, pcliaddr, &len);
        sendto(sock, mesg, n, 0, pcliaddr, len);
        printf("%s\n", mesg);
        printf("%d\n", n);
    }
}