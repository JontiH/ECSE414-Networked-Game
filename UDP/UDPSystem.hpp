#ifndef UDPSystem_INCLUDE
#define UDPSystem_INCLUDE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>

#define BUFFER_LEN 500000
#define SEND_TIMEOUT 10000

struct messageContainer{
    int player;
    char *msg;
};

class UDPSystem
{
    int receivingSocket;
    int msgValue;
    int receivingValue;
    int nameValue;
    int numClient;
    char *m_destIP;
    char m_msg[BUFFER_LEN];
    char *m_portNumber;
    char clientHost[NI_MAXHOST];
    char clientIP[2][NI_MAXHOST];
    addrinfo hints, *receivingInfo, *i;
    sockaddr_storage client_addr, client_storage[2];
    socklen_t client_len = sizeof client_addr;
    socklen_t client_storage_len;
    timeval timeOut;
    messageContainer playerMessage;
    fd_set readfds;
    fd_set writefds;

    void initialSetup();
    int getInfo();
    int createSocket();

public:
    //destIP = NULL for server. otherwise it is the server's ip for clients.
    explicit UDPSystem(char *destIP, char *portNumber);

    void init();
    void connect();
    int getClients();
    void sendPacket(int ,char *);
    messageContainer recvPacket(int );
    void closeSocket();

};

#endif
