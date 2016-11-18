#include "UDPSystem.hpp"

//REFERENCE TO sockaddr_in
 //struct sockaddr_in {
  //  sa_family_t    sin_family; /* address family: AF_INET */
//   in_port_t      sin_port;   /* port in network byte order */ to convert from network byte order,

//use
//ntohs() network byte order to host short

//ntohl() network byte order to host long



  //  struct in_addr sin_addr;   /* internet address */
//};

///* Internet address. */
//struct in_addr {
 //   uint32_t       s_addr;     /* address in network byte order */
//};

//Constructor
// in:: destIp: char* Destination IP address
// in:: portNumber: char * Destination Port
UDPSystem::UDPSystem(char *destIP, char *portNumber) :
    m_destIP(destIP), m_portNumber(portNumber)
{
}

// in: void
// out: void
// This function performs an initializaiton of the object hints of structure type addrinfo
void UDPSystem::initialSetup()
{
    
    memset(&hints, 0, sizeof hints);// fills the first n(sizeof hints) bytes of the memo
    hints.ai_family = AF_UNSPEC; //allow IPV4 or IPV6
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket  UDP*/
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;//This field specifies the protocol for the returned socket addresses. Specifying 0 in this field indicates that socket addresses with any protocol can be returned by getaddrinfo().
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    
}
//in: void
//out: void
// getaddrinfo  initialized addrinfo structures, eachof which contains an Internet address that can be specified in a call
//of which contains an Internet address that can be specified in a call
//to bind(2) or connect(2)
int UDPSystem::getInfo()
{
    
    //receivingInfo is a pointer to a struct addrinfo
    //hints is a struct addrinfo object
    if((receivingValue = getaddrinfo(NULL, m_portNumber, &hints, &receivingInfo)) != 0)
    {
        fprintf(stderr, "UDPSystem::getaddrinfo(): %s\n", gai_strerror(receivingValue));
        return 1;
    }
    return 0;
}
///* getaddrinfo() sets a list of address structures.
//Try each address to a addrinfo structure until we successfully bind(2).
//If socket() or bind() fails, the socket is closed and we try the next address. */

int UDPSystem::createSocket()
{
    for(i = receivingInfo; i != NULL; i = i->ai_next)
    {
        if((receivingSocket = socket(i->ai_family, i->ai_socktype, i->ai_protocol)) == -1)
        {
            perror("Warning: UDPSystem::socket():");
            continue;
        }
        fcntl(receivingSocket, F_SETFL, O_NONBLOCK);
        if(bind(receivingSocket, i->ai_addr, i->ai_addrlen) == -1)
        {
            perror("Warning: UDPSystem::bind():");
            continue;
                        
        }
        break;
    }
    if(i == NULL)
    {
        fprintf(stderr, "UDPSystem::()::createSocket(): Failed to bind socket\n");
        return 1;
    }
    return 0;
}

void UDPSystem::init()
{
    UDPSystem::initialSetup();
    if(UDPSystem::getInfo() == 1)
    {
        //TODO: Error debugging
    }
    if(UDPSystem::createSocket() == 1)
    {
        //TODO: Error debugging
    }
    freeaddrinfo(receivingInfo);
    clientIP[0][0] = '\0';
    clientIP[1][0] = '\0';
}

char * UDPSystem::recvPacket(int timeOutValue)
{
    FD_ZERO(&readfds);
    FD_SET(receivingSocket, &readfds);
    int n = receivingSocket + 1;
    timeOut.tv_sec = 0;
    timeOut.tv_usec = timeOutValue;
    
    select(n, &readfds, NULL, NULL, &timeOut);
    if(FD_ISSET(receivingSocket, &readfds))
    {
        if((msgValue = recvfrom(receivingSocket, m_msg, BUFFER_LEN-1, 0, \
                        (sockaddr *)&client_addr, &client_len)) == -1)
        {
            perror("UDPSystem::recvfrom()");
        }            
        if((nameValue = getnameinfo((sockaddr *) &client_addr, client_len, \
                        clientHost, NI_MAXHOST, NULL, 0, NI_NUMERICHOST)) != 0)
        {
            //TODO: Error debugging.
        }
        if(clientIP[0][0] != '\0')
        {
            if(clientIP[1][0] == '\0')
            {
                if(strcmp(clientHost, clientIP[0]) != 0)
                {
                    memcpy(clientIP[1], clientHost, strlen(clientHost)+1);
                    memcpy(&client_storage[1], &client_addr, sizeof(client_addr));
                }
            }
        }
        else
        {   
            memcpy(clientIP[0], clientHost, strlen(clientHost)+1);
            memcpy(&client_storage[0], &client_addr, sizeof(client_addr));
        }
    }
    return m_msg;
}

void UDPSystem::sendPacket(char *msg)
{    
    FD_ZERO(&writefds);
    FD_SET(receivingSocket, &writefds);
    int n = receivingSocket + 1;
    timeOut.tv_sec = 0;
    timeOut.tv_usec = SEND_TIMEOUT;

    if(clientIP[0][0] != '\0')
    {
        if(clientIP[1][0] != '\0')
        {
            numClient = 2;
        }
        numClient = 1;
    }
    else
    {
        numClient = 0;
    }
    
    for(int k = 0; k < numClient; k++)
    {
        select(n, NULL , &writefds, NULL, &timeOut);
        if(FD_ISSET(receivingSocket, &writefds))
        {
            client_storage_len = sizeof client_storage[k];
            if(sendto(receivingSocket, msg, strlen(msg), 0, \
                        (sockaddr *)&client_storage[k], client_storage_len) == -1)
            {   
                perror("UDPSystem::sendPacket()");
            }   
        }
    }
}

void UDPSystem::connect()
{
    char tempmsg[] = "start";
    sockaddr_in initialAddr; 
    socklen_t initialAddrLen = sizeof initialAddr;
    initialAddr.sin_family = AF_INET;
    initialAddr.sin_port = htons(atoi(m_portNumber));
    inet_pton(AF_INET, m_destIP, &(initialAddr.sin_addr));
    memset(initialAddr.sin_zero, '\0', sizeof initialAddr.sin_zero);

    if(sendto(receivingSocket, tempmsg, strlen(tempmsg), 0, \
               (sockaddr *)&initialAddr, initialAddrLen) == -1)
    {   
        perror("UDPSystem::connect()");
    }
}

void UDPSystem::closeSocket()
{
    close(receivingSocket);
}
