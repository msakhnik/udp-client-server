#include "client.h"
#include "lib/udpOverTcp.h"

int CreateConnection(SAI* ptr_addr, int size)
{
    /* establish connection with server */
    int sock = 0;
    sock = UOTconnect(sock, (struct sockaddr *) ptr_addr, size);
    if (sock < 0)
    {
        close(sock);
        perror("error connecting stream socket");
        exit(1);
    }
    printf("Connection established\n");

    return sock;
}

void GetServerHost(SAI *addr, int size, char** data)
{
    if (size < 3)
    {
        printf("usage : %s <server> <data1> ... <dataN> \n", data[0]);
        exit(1);
    }

    struct hostent *h;
    h = gethostbyname(data[1]);
    if (h == NULL)
    {
        printf("%s: unknown host '%s' \n", data[0], data[1]);
        exit(1);
    }

    printf("%s: sending data to '%s' (IP : %s) \n", data[0], h->h_name,
           inet_ntoa(*(struct in_addr *) h->h_addr_list[0]));

    addr->sin_family = h->h_addrtype;
    memcpy((char *) &addr->sin_addr.s_addr,
           h->h_addr_list[0], h->h_length);
    /*
        addr->sin_port = htons(1500);
     */
}

int _IsReadable(int sd, int * error, int timeOut)
{
    fd_set socketReadSet;
    FD_ZERO(&socketReadSet);
    FD_SET(sd, &socketReadSet);
    struct timeval tv;
    if (timeOut)
    {
        tv.tv_sec = timeOut / 1000;
        tv.tv_usec = (timeOut % 1000) * 1000;
    }
    else
    {
        tv.tv_sec = 0;
        tv.tv_usec = 0;
    }
    if (select(sd + 1, &socketReadSet, 0, 0, &tv) == -1)
    {
        *error = 1;
        return 0;
    } // if
    *error = 0;
    return FD_ISSET(sd, &socketReadSet) != 0;
}

void _SendTo(int sock, char* line,
             SA * remoteServer, size_t size, int position)
{
    int rc = 0;
    rc = sendto(sock, line, strlen(line) + 1, 0,
                (SA *) remoteServer, size);
    if (rc < 0)
    {
        printf("Cannot send data %d ", position);
        perror("");
        close(sock);
        exit(1);
    }
}

void SendMessage(int sock, SA * remoteServer,
                 size_t size_serv, int argc, char** argv)
{
    int n = 0;
    SAI echoAddr;
    char mesg[MAXLINE];
    int i = 0;
    for (i = 2; i < argc; i++)
    {
        UOTsend(sock, argv[i], sizeof (argv[i]), 0);

        memset(mesg, 0x0, MAXLINE);
        //this const value - max time for waiting answer from server
        int timeOut = 1000; // ms
        int error = 0;
        /*
                while (!_IsReadable(sock, &error, timeOut))
         */
        fprintf(stdout, "This%d\n", sock);
        UOTsend(sock, argv[i], sizeof (argv[i]), 0);
        printf("\n");
        int echoLen = sizeof (echoAddr);
        perror("before ret'");
        n = UOTrecv(sock, mesg, MAXLINE, 0);
        /*
                n = recvfrom(sock, mesg, MAXLINE, 0,
                             (SA *) &echoAddr, &echoLen);
         */
        if (n < 0)
        {
            perror("Cannot receive data");
            continue;
        }
        printf("Echo from %s:UDP%u : %s \n", inet_ntoa(echoAddr.sin_addr),
               ntohs(echoAddr.sin_port), mesg);
    }
}

void CloseConnection(int sock)
{
    printf("\nClose Socket\n");
    close(sock);
}