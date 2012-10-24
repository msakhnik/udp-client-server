/* 
 * File:   main.c
 * Author: morwin
 *
 * Created on 23 жовтня 2012, 19:19
 */

#include <stdio.h>
#include <stdlib.h>

#include "server.h"

/*
 * 
 */
int main(int argc, char** argv) {
    
    int sock;
    struct sockaddr_in addr;

    sock = CreateConnection((SA *) &addr, sizeof(addr));
    if (sock == -1)
    {
        return -1;
    }

    ReciveData(sock);

    CloseConnection(sock);

    return 0;
}

