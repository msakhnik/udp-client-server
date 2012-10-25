#include <stdio.h>
#include <stdlib.h>

#include "server.h"

int main(int argc, char** argv)
{

    int sock;
    SAI addr, cliaddr;

    sock = CreateConnection((SAI *) & addr, sizeof (addr));

    RunEcho(sock, sizeof (cliaddr));

    CloseConnection(sock);

    return 0;
}