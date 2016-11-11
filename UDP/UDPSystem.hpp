#ifndef UDPSystem_INCLUDE
#define UDPSystem_INCLUDE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFFER_LEN 5000

class UDPSystem
{
    int socketDesc;
    int msgValue;
    int infoValue;
    char *m_destIP;
    char m_msg[BUFFER_LEN];
    char *m_portNumber;
    addrinfo hints, *serviceInfo, *i;
    sockaddr_storage client_addr;
    socklen_t client_len = sizeof client_addr;

    void initialSetup();
    int getInfo(bool );
    int createSocket(bool );

public:
    explicit UDPSystem(char *destIP, char *portNumber);

    void sendPacket(char *);
    char * recvPacket();
};

#endif
