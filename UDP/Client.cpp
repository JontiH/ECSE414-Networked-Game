/*
 * Client.cpp is used to send simple message to a specific ip address via UPD packets.
 * Useful for testing UDP connections.
 * Port number is hardcoded below.
 * Built using Beej's guide to Network programming. 
 */

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

int main(int argc, char *argv[]){

    int socketdesc;
    addrinfo hints, *serviceinfo, *i; //addrinfo is a struct that will contain all the info for a connection.
    int infovalue;
    int msg;
    char* serverport = argv[2];

    if(argc != 4){
        fprintf(stderr,"Client: Missing arguments\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints); //must clear info from previous calls
    hints.ai_family = AF_UNSPEC; //Set ip to unspecified so it will allow us to use both ipv4 and ipv6
    hints.ai_socktype = SOCK_DGRAM;

    /* getaddrinfo returns 0 on success, catch all errors when getting host info here (if any).
     * gai_strerror() return a printable version of the error code.
     * If it is successful, store all the result in serviceinfo. */
    if((infovalue = getaddrinfo(argv[1], serverport, &hints, &serviceinfo)) != 0){
        fprintf(stderr, "Client getaddrinfo(): %s\n", gai_strerror(infovalue));
        return 1;
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
        return 2;
    }

    /* sendto() returns the number of bytes sent, might be less then what you asked it to send.
     * Also return -1 on error and sets errno to the error's value. 
     * We use perror to print the most recent errno. */
    if((msg = sendto(socketdesc, argv[3], strlen(argv[3]), 0, i->ai_addr, i->ai_addrlen)) == -1){
        perror("Client: sendto");
        exit(1);
    }
    
    freeaddrinfo(serviceinfo);

    printf("Client: sent %d byte(s) to %s/%s\n", msg, argv[1], argv[2]);
    close(socketdesc);
    return 0;
}
