#include "server.h"

int CreateConnection(SA* ptr_addr, int size)
{
    
    int sock = 0;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }
    
    ptr_addr->sin_family = AF_INET;
    ptr_addr->sin_port = htons(3425);
    ptr_addr->sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sock,  (struct sockaddr *)ptr_addr, size) < 0)
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
    }
}

void CloseConnection(int sock)
{
    printf("\nClose Socket\n");
    close(sock);
}