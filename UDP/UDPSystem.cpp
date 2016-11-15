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
    if((receivingValue = getaddrinfo(NULL, m_portNumber, &hints, &receivingInfo)) != 0)
    {
        fprintf(stderr, "UDPSystem::getaddrinfo(): %s\n", gai_strerror(receivingValue));
        return 1;
    }
    if((sendingValue = getaddrinfo(m_destIP, m_portNumber, &hints, &sendingInfo)) != 0)
    {
        fprintf(stderr, "UDPSystem::getaddrinfo(): %s\n", gai_strerror(sendingValue));
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
    for(j = sendingInfo; j != NULL; j = j->ai_next)
    {
        if((sendingSocket = socket(j->ai_family, j->ai_socktype, j->ai_protocol)) == -1)
        {
            perror("Warning: UDPSystem::createsocket():");
            continue;
        }
        break;
    }
    if((i == NULL) || (j == NULL))
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
    freeaddrinfo(sendingInfo);
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
    }
    return m_msg;
}

void UDPSystem::sendPacket(char *msg)
{
    if(sendto(sendingSocket, msg, strlen(msg), 0, i->ai_addr, i->ai_addrlen) == -1)
    {
        perror("UDPSystem::sendPacket()");
    }
}

void UDPSystem::closeSocket()
{
    close(receivingSocket);
    close(sendingSocket);
}
