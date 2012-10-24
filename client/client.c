#include "client.h"

int CreateConnection(SA* ptr_addr, int size)
{
    int sock = 0;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        perror("socket");
        return -1;
    }
    ptr_addr->sin_family = AF_INET;
    ptr_addr->sin_port = htons(3425);
    ptr_addr->sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (connect(sock, (struct sockaddr *)ptr_addr, size) < 0)
    {
        perror("perror");
        return -1;
    }

    return sock;
}

void SendMessage(SA* ptr_addr, int sock, char * msg, int size)
{
    send(sock, msg, size, 0);
}

void CloseConnection(int sock)
{
    printf("\nClose Socket\n");
    close(sock);
}