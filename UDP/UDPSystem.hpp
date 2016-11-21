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
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>

#define BUFFER_LEN 500 // Buffer size
#define SEND_TIMEOUT 125000

class UDPSystem
{
    int receivingSocket;
    int msgValue; //size of the packet
    int receivingValue;
    int nameValue;
    int numClient;
    char *m_destIP;
    char m_msg[BUFFER_LEN];
    char *m_portNumber;
    char clientHost[NI_MAXHOST]; //NI_MAXHOST size is 1025 s
    char clientIP[2][NI_MAXHOST];
    char ip[INET6_ADDRSTRLEN];
    addrinfo hints, *receivingInfo, *i;
    sockaddr_storage client_addr, client_storage[2];
    socklen_t client_len;
    socklen_t client_storage_len[2];
    timeval timeOut;
    fd_set readfds;
    fd_set writefds;
    
    void initialSetup();
    int getInfo();
    int createSocket();
    
public:
    //destIP = NULL for server. otherwise it is the server's ip.
    explicit UDPSystem(char *destIP, char *portNumber);
    
    void init();
    void connect2(char *);
    void connect(char *);
    void sendPacket(char *, int );
    char * recvPacket(int ,int);
    void closeSocket();
    int  clientsNotConnected();
    
   
    
    
};

#endif
