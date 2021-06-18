#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define MAXPENDING 5

int main() {
    /* 1. Create a TCP Socket
    Create socket for incoming connections */
    int servSock;
    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    /* 2. Assign a port to socket */
    struct sockaddr_in servAddress;
    servAddress.sin_family = AF_INET; /* Internet address family */
    servAddress.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    servAddress.sin_port = htons(servAddress); /* Local port */
    if (bind(servSock, (struct sockaddr *) &servAddress, sizeof(servAddress)) < 0)
        DieWithError("bind() failed");

    /* 3. Set socket to listen */
    /* Mark the socket so it will listen for incoming connections */
    if (listen(servSock, MAXPENDING) < 0)
        DieWithError("listen() failed");

    /* 4a. Accept new connection */
    for (;;) /* Run forever */
    {
    clntLen = sizeof(echoClntAddr);
    if ((clientSock=accept(servSock,(struct sockaddr *)&echoClntAddr,&clntLen))<0)
    DieWithError("accept() failed");
    }