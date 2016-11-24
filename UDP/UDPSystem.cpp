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
    return 0;
}

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

UDPSystem::messageContainer UDPSystem::recvPacket(int timeOutValue)
{
    //clear the struct
    playerMessage.player = 0;
    playerMessage.msg = NULL;

    FD_ZERO(&readfds); //clear the set readfds
    FD_SET(receivingSocket, &readfds); //add receivingSocket to the set
    int n = receivingSocket + 1; //select()'s manpage says to add 1, so we add 1...
    
    //initialize the timeout value for select()
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
                    playerMessage.player = 2;
                }
                else
                {
                    playerMessage.player = 1;
                }
            }
            else
            {
                if(strcmp(clientHost, clientIP[0]) != 0)
                {
                    playerMessage.player = 2;
                }
                else
                {
                    playerMessage.player = 1;
                }
            }
        }
        else
        {   
            memcpy(clientIP[0], clientHost, strlen(clientHost)+1);
            memcpy(&client_storage[0], &client_addr, sizeof(client_addr));
            playerMessage.player = 1;
        }
    }
    playerMessage.msg = m_msg;
    return playerMessage;
}

void UDPSystem::sendPacket(int player, char *msg)
{    
    FD_ZERO(&writefds);
    FD_SET(receivingSocket, &writefds);
    int n = receivingSocket + 1;
    timeOut.tv_sec = 0;
    timeOut.tv_usec = SEND_TIMEOUT;
    int index;

    switch(player)
    {
        case 1: index = 0; 
                numClient = 1;
                break;
        case 2: index = 1;
                numClient = 2;
                break;
        default: index = 0;
                 numClient = 2;
                 break;

    }
   
    while(index < numClient)
    {
        select(n, NULL , &writefds, NULL, &timeOut);
        if(FD_ISSET(receivingSocket, &writefds))
        {
            client_storage_len = sizeof client_storage[index];
            if(sendto(receivingSocket, msg, strlen(msg), 0, \
                        (sockaddr *)&client_storage[index], client_storage_len) == -1)
            {   
                perror("UDPSystem::sendPacket()");
            }   
        }
        index++;
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
