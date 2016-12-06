#include "UDPSystem.hpp"

/*
 *  UDPSystem constructor.
 */
UDPSystem::UDPSystem(char *destIP, char *portNumber) : 
    m_destIP(destIP), m_portNumber(portNumber)
{
}

/*
 *  initialSetup() is used to set some variable prior to a call to getInfo()
 *  @param none
 *  @return void
 */
void UDPSystem::initialSetup()
{
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
}

/*
 *  getInfo() create a structure that will then be used to create a socket.
 *  The structure is based on an IP (NULL), a port number and a similar struct
 *  (hints) which contains criteria for creating a socket. The resulting struct
 *  is stored in receivingInfo. The function return 0 on success or 1 on error.
 *  @param none
 *  @return int
 */
int UDPSystem::getInfo()
{
    if((receivingValue = getaddrinfo(NULL, m_portNumber, &hints, &receivingInfo)) != 0)
    {
        fprintf(stderr, "UDPSystem::getaddrinfo(): %s\n", gai_strerror(receivingValue));
        return 1;
    }
    return 0;
}

/*
 *  createSocket() creates and bind a socket to a port based on the info
 *  stored in receivingInfo. It also sets the socket to O_NONBLOCK to 
 *  prevent it from blocking (when calling recvfrom and sendto on it)
 *  and set its memory address to SO_REUSEADDR to allows us to 
 *  quickly rebinds a new socket in case of failure. The function return
 *  0 on success or 1 on error.
 *  @param none
 *  @return int
 *
 */
int UDPSystem::createSocket()
{
    int option = 1;
    for(i = receivingInfo; i != NULL; i = i->ai_next)
    {
        if((receivingSocket = socket(i->ai_family, i->ai_socktype, i->ai_protocol)) == -1)
        {
            perror("Warning: UDPSystem::socket():");
            continue;
        }
        fcntl(receivingSocket, F_SETFL, O_NONBLOCK);
        if (setsockopt(receivingSocket,SOL_SOCKET,SO_REUSEADDR,&option,sizeof option) == -1) {
                perror("UDPSystem::setsockopt():");
        } 
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

/*
 *  init() is a public function that prepares the UDPSystem object before uses.
 *  @param none
 *  @return void
 */
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

/*
 *  recvPacket() is a public function used to read a packet from the socket's queue.
 *  It verifies if the socket is ready to read. Then attempt to get the 
 *  first packet from it. Then it gets the packet's source IP and Port from
 *  the udp "pseudo header" and store the information if its for further
 *  sendPacket() calls if necessary (i.e. if not already stored). It takes
 *  a timeout value as parameter in microseconds and return a struct
 *  containing the packet's message and player id.
 *  @param int timeOutValue
 *  @return struct messageContainer
 */
messageContainer UDPSystem::recvPacket(int timeOutValue)
{
    playerMessage.player = 0;
    playerMessage.msg = NULL;
    m_msg[0] = '\0';

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
    if(m_msg[0] == '\0')
    {
        playerMessage.msg = NULL;
    }
    else
    {
        m_msg[msgValue] = '\0';
        playerMessage.msg = m_msg; 
    }
    return playerMessage;
}

/*
 *  sendPacket() is a public function used to send a message to the previously stored client(s).
 *  It first checks to which client to send to based on the parameter 'player'.
 *  Then is verifies if the socket is ready to read and if so it sends the msg.
 *  @param int player, char *msg
 *  @return void
 */
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

/*
 *  connect() is a public function used by the clients during the udp 'handshake' to send a message
 *  to the server without having to rely on stored information. It also creates 
 *  the necessary structure by hands instead of using getInfo() or getaddrinfo().
 *  @param none
 *  @return void
 */
void UDPSystem::connect()
{
    char connectValue[] = "start";
    sockaddr_in initialAddr; 
    socklen_t initialAddrLen = sizeof initialAddr;
    initialAddr.sin_family = AF_INET;
    initialAddr.sin_port = htons(atoi(m_portNumber));
    inet_pton(AF_INET, m_destIP, &(initialAddr.sin_addr));
    memset(initialAddr.sin_zero, '\0', sizeof initialAddr.sin_zero);

    if(sendto(receivingSocket, connectValue, strlen(connectValue), 0, \
               (sockaddr *)&initialAddr, initialAddrLen) == -1)
    {   
        perror("UDPSystem::connect()");
    }
}

/*
 *  getClients() is a public helper function used to retrieve the number of 
 *  stored clients. It returns the number of non-NULL IP stored.
 *  @param none
 *  @return int
 */
int UDPSystem::getClients()
{
    if(clientIP[0][0] != '\0')
    {
        if(clientIP[1][0] != '\0')
        {
            return 2;
        }
        return 1;
    }
    return 0;
}

/*
 *  closeSocket() is a public function used to close the socket create 
 *  in the UDPSystem object. It ends any further connections.
 *  @param none
 *  @return void
 */
void UDPSystem::closeSocket()
{
    close(receivingSocket);
}
