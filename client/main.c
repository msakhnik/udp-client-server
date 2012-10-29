/*
* File: main.c
* Author: morwin
*
* Created on 23 жовтня 2012, 16:06
*/
/*
#include <stdio.h>
#include <stdlib.h>
#include "client.h"

int main(int argc, char** argv)
{
    char msg1[] = "Hello there!\n";
    char msg2[] = "Bye bye!\n";
    int sock;
    struct sockaddr_in addr;

    sock = CreateConnection((SAI *) & addr, sizeof (addr));
    if (sock == -1)
    {
        return -1;
    }


    SendMessage(stdin, sock, (SA *) & addr, sizeof (addr));

    CloseConnection(sock);
    return 0;
}*/

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


