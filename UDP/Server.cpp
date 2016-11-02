/*
 * Server.cpp is used to receive simple message via UDP packets.
 * It then prints the sender's info and message.
 * Useful for testing UDP connections.
 * Port number is hardcoded below.
 * Built using Beej's guide to Network programming.
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUFFERLEN 1000

/* *get_in_addr() is used to return the appriopriate value if the ip of the client is ipv4 or ipv6 */
void *get_in_addr(sockaddr *sa){
    
    if(sa->sa_family = AF_INET){
        return &(((sockaddr_in*)sa)->sin_addr);
    }
    return &(((sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[]){

    int socketdesc;
    addrinfo hints, *serviceinfo, *i; //addrinfo is a struct that contains all the info for a connection.
    int infovalue;
    int msg_size;
    sockaddr_storage client_addr; //used as a buffer to hold the client's info.
    socklen_t client_len = sizeof client_addr; 
    /* ip is set to length INET6_ADDRSTRLEN since it may contain either ipv4 or ipv6 */
    char ip[INET6_ADDRSTRLEN];
    char msg[BUFFERLEN];
    char* localport = argv[1];

    memset(&hints, 0, sizeof hints); //must clear info from previous calls.
    hints.ai_family = AF_UNSPEC; //set ip to unspecified so it will allow us to use both ipv4 and ipv6
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; //this tells getaddrinfo() to assign the address of localhost to the socket structure.

    if ((infovalue = getaddrinfo(NULL, localport, &hints, &serviceinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(infovalue));
        return 1;
    }

    /* Loop through all the resut in serviceinfo unti it is successful.
     * socket() return -1 on error and set errno to the error's value.
     * We use perror to print the most recent errno.
     * The first time it is successful, it creates a socket and store the file descriptor in socketdesc. 
     * Also binds a socket to a specific port (used to listen on that port).
     * bind() return -1 on error and set errno to the error's value... */
    for(i = serviceinfo; i != NULL; i = i->ai_next) {
        if ((socketdesc = socket(i->ai_family, i->ai_socktype, i->ai_protocol)) == -1) {
            perror("Server: socket");
            continue;
        }

        if (bind(socketdesc, i->ai_addr, i->ai_addrlen) == -1) {
            close(socketdesc);
            perror("Server: bind");
            continue;
        }

        break;
    }
    if (i == NULL) {
        fprintf(stderr, "Server: failed to bind socket\n");
        return 2;
    }
    freeaddrinfo(serviceinfo);

    printf("Server: Waiting to recvfrom()...\n");

    if ((msg_size = recvfrom(socketdesc, msg, BUFFERLEN-1 , 0, (sockaddr *)&client_addr, &client_len)) == -1) {
        perror("recvfrom():");
        exit(1);
    }

    printf("Server: got packet from %s\n", inet_ntop(client_addr.ss_family, 
                get_in_addr((sockaddr *)&client_addr), ip, sizeof ip));
    printf("Server: packet is %d bytes long\n", msg_size);
    msg[msg_size] = '\0';
    printf("Server: packet contains \"%s\"\n", msg);

    close(socketdesc);

    return 0;
}
