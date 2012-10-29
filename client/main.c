#include <stdio.h>
#include <stdlib.h>
#include "client.h"

int main(int argc, char** argv)
{
    
    int sock;
    SAI addr, remoteServer;

    GetServerHost((SAI *) & remoteServer, argc, argv);
    sock = CreateConnection((SAI *) & addr, sizeof (addr));

    SendMessage(sock, (SA *) & remoteServer,
                           sizeof (remoteServer), argc, argv);

    CloseConnection(sock);
    return 0;
}


