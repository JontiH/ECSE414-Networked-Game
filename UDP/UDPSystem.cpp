#include "UDPSystem.hpp"

UDPSystem::UDPSystem(char *destIP, char *portNumber) : 
    m_destIP(destIP), m_portNumber(portNumber)
{
}

void UDPSystem::sendPacket(char *msg)
{
    UDPSystem::initialSetup();
    if(UDPSystem::getInfo(true) != 0)
    {
        //TODO: add error debugging      
    }
    if(UDPSystem::createSocket(true) != 0)
    {
        //TODO: add error debugging
    }
    if(sendto(socketDesc, msg, strlen(msg), 0, i->ai_addr, i->ai_addrlen) == -1)
    {
        perror("UDPSystem::sendPacket()");
        exit(EXIT_FAILURE);
    }
    freeaddrinfo(serviceInfo);
    close(socketDesc);
}
void UDPSystem::initialSetup()
{
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
}

int UDPSystem::getInfo(bool isSending)
{
    if(!isSending)
    {
        if((infoValue = getaddrinfo(NULL, m_portNumber, &hints, &serviceInfo)) != 0)
        {
            fprintf(stderr, "UDPSystem::recvPacket()::getInfo(true): %s\n", gai_strerror(infoValue));
            return 1;
        }
    }
    else
    {
        if((infoValue = getaddrinfo(m_destIP, m_portNumber, &hints, &serviceInfo)) != 0)
        {
            fprintf(stderr, "UDPSystem::sendPacket()::getInfo(false): %s\n", gai_strerror(infoValue));
            return 1;
        }
    }
    return 0;
}

int UDPSystem::createSocket(bool isSending)
{
    for(i = serviceInfo; i != NULL; i = i->ai_next)
    {
        if((socketDesc = socket(i->ai_family, i->ai_socktype, i->ai_protocol)) == -1)
        {
            perror("Warning: UDPSystem::sendPacket()::createsocket():");
            continue;
        }
        if(!isSending){
            if(bind(socketDesc, i->ai_addr, i->ai_addrlen) == -1)
                    {
                        close(socketDesc);
                        perror("Warning: UDPSystem::recvPacket()::createsocket(): bind():");
                        continue;
                        
                    }
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

char * UDPSystem::recvPacket()
{
    UDPSystem::initialSetup();
    if(UDPSystem::getInfo(false) != 0)
        {
            //TODO: add error debugging      
        }
    if(UDPSystem::createSocket(false) != 0)
        {
            //TODO: add error debugging
        }
    freeaddrinfo(serviceInfo);
    if((msgValue = recvfrom(socketDesc, m_msg, BUFFER_LEN-1, 0, (sockaddr *)&client_addr, &client_len)) == -1)
    {
        perror("UDPSystem::recvPacket()::recvfrom()");
        exit(EXIT_FAILURE);
    }
    close(socketDesc);

    return m_msg;
}
