#include "UDPSystem.hpp"

UDPSystem::UDPSystem(char *destIP, char *portNumber) : 
    m_destIP(destIP), m_portNumber(portNumber)
{
}

void UDPSystem::initialSetup()
{
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
}

int UDPSystem::getInfo()
{
    if((receivingValue = getaddrinfo(m_destIP, m_portNumber, &hints, &receivingInfo)) != 0)
    {
        fprintf(stderr, "UDPSystem::getaddrinfo(): %s\n", gai_strerror(receivingValue));
        return 1;
    }
    return 0;
}

int UDPSystem::createSocket()
{
    for(i = receivingInfo; i != NULL; i = i->ai_next)
    {
        if((receivingSocket = socket(i->ai_family, i->ai_socktype, i->ai_protocol)) == -1)
        {
            perror("Warning: UDPSystem::createsocket():");
            continue;
        }
        fcntl(receivingSocket, F_SETFL, O_NONBLOCK);
        if(bind(receivingSocket, i->ai_addr, i->ai_addrlen) == -1)
        {
            perror("Warning: UDPSystem::recvPacket()::createsocket(): bind():");
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
    if(sendto(receivingSocket, tempmsg, strlen(tempmsg), 0, \
               i->ai_addr, i->ai_addrlen) == -1)
    {   
        perror("UDPSystem::connect()");
    }
}

void UDPSystem::closeSocket()
{
    close(receivingSocket);
}
