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
int num_clients = 1;
// in: void
// out: void
// This function performs an initializaiton of the object hints of structure type addrinfo
void UDPSystem::initialSetup()
{
    
    memset(&hints, 0, sizeof hints);// fills the first n(sizeof hints) bytes of the memo
    hints.ai_family = AF_INET; //allow IPV4 or IPV6
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
        /* setsockopt: Handy debugging trick that lets
         * us rerun the server immediately after we kill it;
         * otherwise we have to wait about 20 secs.
         * Eliminates "ERROR on binding: Address already in use" error.
         */
        int optval = 1;
        setsockopt(receivingSocket, SOL_SOCKET, SO_REUSEADDR,
                   (const void *)&optval , sizeof(int));
        //fcntl(receivingSocket, F_SETFL, O_NONBLOCK);// set to non-blocking file descriptor

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
    client_len = sizeof client_addr;
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

char * UDPSystem::recvPacket(int timeOutValue, int connecting)
{
    // FD_ZERO(&readfds);
    // FD_SET(receivingSocket, &readfds);
    // int n = receivingSocket + 1;
    // timeOut.tv_sec = 0;
    //timeOut.tv_usec = timeOutValue;
    
    // select(n, &readfds, NULL, NULL, &timeOut);
    //if(FD_ISSET(receivingSocket, &readfds))
    // {
    
    
    if((msgValue = recvfrom(receivingSocket, m_msg, BUFFER_LEN-1, 0, \
                            (sockaddr *)&client_addr, &client_len)) == -1)
    {
        perror("UDPSystem::recvfrom()");
    }
    if((nameValue = getnameinfo((sockaddr *) &client_addr, client_len, \
                                clientHost, NI_MAXHOST, NULL, 0, NI_NUMERICHOST)) != 0)
    {
        //TODO: Error debugging.
        perror("UDPSystem::getNameInfo()");
    }
    if(num_clients == 1)
    {
        client_storage_len[0] = client_len;
    }
    if(num_clients == 2)
    {
        client_storage_len[1] = client_len;
    }
    if(clientIP[0][0] != '\0')
    {
        if(clientIP[1][0] == '\0')
        {
            //if(strcmp(clientHost, clientIP[0]) == 0)//waits for a second client with different IPchange to != 0
            //{
            //set the port of the incoming packet to m_portNumber so that the client will catch the packet at port m_portNumber and not a random port
            struct sockaddr_in * sin;
            sin = (struct sockaddr_in *)&client_addr;
            sin->sin_port = htons(atoi(m_portNumber));
            memcpy(clientIP[1], clientHost, strlen(clientHost)+1);
            memcpy(&client_storage[1], &client_addr, client_storage_len[1]);
            
            printf("Client1 %s, Client2 %s \n",clientIP[0], clientIP[1]);
            // }
        }
        
    }
    else
    {
        //set the port of the incoming packet to m_portNumber so that the client will catch the packet at port m_portNumber and not a random port
        struct sockaddr_in * sin;
        sin = (struct sockaddr_in *)&client_addr;
        sin->sin_port = htons(atoi(m_portNumber));
        memcpy(clientIP[0], clientHost, strlen(clientHost)+1);
        memcpy(&client_storage[0], &client_addr, client_storage_len[0]);
        
        // printf("case2\n");
    }
    //}
    m_msg[msgValue] = '\0';
    printf("CLIENTS CONNECTED %d\n", num_clients);
    if(connecting == 1){
        num_clients = num_clients+1;}
    return m_msg;
}
void *get_in_addr(sockaddr *sa){
    
    if(sa->sa_family == AF_INET){
        
        return &(((sockaddr_in*)sa)->sin_addr);
    }
    return &(((sockaddr_in6*)sa)->sin6_addr);
}
void UDPSystem::sendPacket(char *msg, int givePlayerID)
{
    //FD_ZERO(&writefds);
    //FD_SET(receivingSocket, &writefds);
    //int n = receivingSocket + 1;
    // timeOut.tv_sec = 0;
    // timeOut.tv_usec = SEND_TIMEOUT;
    
    if(clientIP[0][0] != '\0')
    {
        if(clientIP[1][0] != '\0')
        {
            numClient = 2;
            
        }
        if(clientIP[1][0] == '\0'){numClient = 1;}
    }
    else
    {
        numClient = 0;
    }
    if(givePlayerID == 0){
        for(int k = 0; k < num_clients-1; k++)
        {
            //select(n, NULL , &writefds, NULL, &timeOut);
            // if(FD_ISSET(receivingSocket, &writefds))
            // {
            
            int msg2;
            //client_storage_len = sizeof client_storage[k];
         //   printf("SIZE 1 %d vs SIZE 2 %d \n", client_storage_len[k],sizeof client_storage[k]);
            //The following line sets the destination port of the incoming packet to m_portNumber since the client will have a socket listening on that port
           
            if((msg2 = sendto(receivingSocket, msg, strlen(msg), 0, \
                              (struct sockaddr *)&client_storage[k], client_storage_len[k]))==-1)
            {
                
                perror("UDPSystem::sendPacket()");
            }
            //smsg[strlen(msg)] = '\0';
            
            printf("SERVER: sent %d byte(s) to %s\n", msg2,inet_ntop(client_storage[k].ss_family,
                                                                     get_in_addr((sockaddr *)&client_storage[k]), ip, sizeof ip));
            
            //}
        }
        
    }
    //the server needs to tell each player which player they are
    if(givePlayerID == 1){
        int msg2;
        char *msg3;
        if(num_clients-1 == 1){
            msg3 = "player1";
        }
        if(num_clients-1==2){
            msg3 = "player2";
        }
        //client_storage_len = sizeof client_storage[k];
        printf("PLAYER %d\n", num_clients-1);
        //-2 because after receiving  a packet, the num of clients is increased by 1.
       // printf("SIZE 1 %d vs SIZE 2 %d \n", client_storage_len[num_clients-2],sizeof client_storage[num_clients-2]);
        
        
        
        
        if((msg2 = sendto(receivingSocket, msg3, strlen(msg3), 0, \
                          (struct sockaddr *)&client_storage[num_clients-2], client_storage_len[num_clients-2]))==-1)
        {
            
            perror("UDPSystem::sendPacket()");
        }
        //smsg[strlen(msg)] = '\0';
        
        printf("SERVER NOTIFICATION TO PLAYER %d: sent %d byte(s) to %s\n", num_clients-1,msg2,inet_ntop(client_storage[num_clients-2].ss_family,
                                                                 get_in_addr((sockaddr *)&client_storage[num_clients-2]), ip, sizeof ip));
        
    }
}

void UDPSystem::connect2(char * tempmsg)
{
    int socketdesc;
    addrinfo hints, *serviceinfo, *i; //addrinfo is a struct that will contain all the info for a connection.
    int infovalue;
    int msg;
    char* serverport = m_portNumber;

    

    memset(&hints, 0, sizeof hints); //must clear info from previous calls
    hints.ai_family = AF_INET; //Set ip to unspecified so it will allow us to use both ipv4 and ipv6
    hints.ai_socktype = SOCK_DGRAM;

    /* getaddrinfo returns 0 on success, catch all errors when getting host info here (if any).
     * gai_strerror() return a printable version of the error code.
     * If it is successful, store all the result in serviceinfo. */
    if((infovalue = getaddrinfo(m_destIP, m_portNumber, &hints, &serviceinfo)) != 0){
        fprintf(stderr, "Client getaddrinfo(): %s\n", gai_strerror(infovalue));
        
    }

    /* Loop through all the result in serviceinfo until it is successful.
     * socket() return -1 on error and set errno to the error's value.
     * We use perror to print the most recent errno. 
     * The first time it is successful, it creates a socket and store the file descriptor in socketdesc. */
    for(i = serviceinfo; i != NULL; i = i->ai_next){
        if((socketdesc = socket(i->ai_family, i->ai_socktype, i->ai_protocol)) == -1){
            perror("Client: socket");
            continue;
        }
        break;
    }

    if(i == NULL){
        fprintf(stderr, "Client: failed to create a socket\n");
        
    }

    /* sendto() returns the number of bytes sent, might be less then what you asked it to send.
     * Also return -1 on error and sets errno to the error's value. 
     * We use perror to print the most recent errno. */
    if((msg = sendto(socketdesc, tempmsg, strlen(tempmsg), 0, i->ai_addr, i->ai_addrlen)) == -1){
        perror("Client: sendto");
        exit(1);
    }
    
    freeaddrinfo(serviceinfo);

    printf("Client: sent %d byte(s) to %s/%s\n", msg ,m_destIP, m_portNumber);
    close(socketdesc);

   
   
}
void UDPSystem::connect(char * tempmsg)
{
    
    
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
int UDPSystem::clientsNotConnected()
{
    return (num_clients<=2);
}
