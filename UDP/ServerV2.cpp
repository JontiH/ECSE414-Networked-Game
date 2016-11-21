//
//  ServerV2.cpp
//
//
//
//
//
//#include "ServerV2.hpp"
#include "UDPSystem.hpp"

int main(int argc, char *argv[]){
    
    
    //TO-DO
    UDPSystem server = UDPSystem(argv[1],argv[2]);
    
    char * msg ;
    int len = 0;
    server.init();
    //Here we wait for 2 connections and we tell each player
    while(server.clientsNotConnected()){
        printf("Server: Waiting for 2 clients...\n");
        msg = server.recvPacket(SEND_TIMEOUT,1);//1 because we are making a connection with the client
        len = strlen(msg);
        printf("Server: packet contains \"%s\"\n",msg);
        
        server.sendPacket(msg,1);// the 1 is to tell each client which player they are
        
    }
   
    
    
    server.closeSocket();
    
    //now forward incoming packets to all players
    //to stop infinite loop use control c
    server.init();
    while(1){
         printf("Server: Waiting for players data..\n");
         msg = server.recvPacket(SEND_TIMEOUT,0);// 0 because we are forwarding packets and we dont tell the clients which player they are
         printf("Server: packet from player contains \"%s\"\n",msg);
        
         server.sendPacket(msg,0);// we are not telling each client which player they are
    }
    
    server.closeSocket();
    
    
    return 0;
}


