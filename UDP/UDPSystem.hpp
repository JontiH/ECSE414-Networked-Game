#ifndef UDPSystem_INCLUDE
#define UDPSystem_INCLUDE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>

#define BUFFER_LEN 5000

class UDPSystem
{
    int sendingSocket;
    int receivingSocket;
    int msgValue;
    int sendingValue;
    int receivingValue;
    char *m_destIP;
    char m_msg[BUFFER_LEN];
    char *m_portNumber;
    addrinfo hints, *sendingInfo, *receivingInfo, *i, *j;
    sockaddr_storage client_addr;
    socklen_t client_len = sizeof client_addr;
    timeval timeOut;
    fd_set readfds;
    fd_set writefds;

    void initialSetup();
    int getInfo();
    int createSocket();

public:
    explicit UDPSystem(char *destIP, char *portNumber);

    void init();
    void sendPacket(char *);
    char * recvPacket(int );
    void closeSocket();

};

#endif
