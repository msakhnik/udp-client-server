#include "client.h"

int CreateConnection(SAI* ptr_addr, int size)
{
    int sock = 0;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        perror("socket");
        return -1;
    }
    ptr_addr->sin_family = AF_INET;
    ptr_addr->sin_port = htons(7);
    ptr_addr->sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (connect(sock, (struct sockaddr *)ptr_addr, size) < 0)
    {
        perror("perror");
        return -1;
    }

    return sock;
}

void SendMessage(FILE * fp, int sock, const SA * pservaddr, size_t size)
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

void CloseConnection(int sock)
{
    printf("\nClose Socket\n");
    close(sock);
}