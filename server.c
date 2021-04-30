//the server program -> handle client socket connections, broadcast etc

//TODO:
//- need to figure out a way to detect when a connection is closed, and determining which specific connection was closed
//  and then removing this USER object from the user list
//- give shell prompt so an admin can list current state output
//- executable should include possible command line arguments which specify the port number

//

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>

#define PORT 8080
#define BACKLOG 3
#define MAX_NUM_CONNECTIONS 5

typedef struct user{
    int user_fd;         //the user's socket fd
    char nickname[15];   //the user's nickname
    int kickvotes;
} USER;

int main(int argc, char **argv){
    int client_fd;
    int server_fd;
    int opt = 1;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    int numConnections = 0;
    USER *userlist[MAX_NUM_CONNECTIONS];    //the list of users currently connected to the server


    //=======================================================
    //Step 1: create a socket file descriptor (IPv4, TCP, IP)
    //=======================================================
    if( (server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        fprintf(stderr, "errno: %d\n", errno);
        perror("Error from perror");
        fprintf(stderr, "socket()\n");
        exit(EXIT_FAILURE);  //exit for now, MUST REMOVE LATER!
    }
    else
        printf("Created socket file descriptor: %d\n", server_fd);
    
    //============================= !!! will need to review socket options here!
    //Step 2: select socket options 
    //=============================
    if( setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0){
        fprintf(stderr, "errno: %d\n", errno);
        perror("Error from perror");
        fprintf(stderr, "setsockopt()\n");
        exit(EXIT_FAILURE);  //exit for now, MUST REMOVE LATER!
    }
    else   
        printf("Set socket options successfully...\n");

    //========================================================
    //Step 3: Bind an address to the previously unnamed socket
    //========================================================
    address.sin_family = AF_INET;          //use IPv4
    address.sin_addr.s_addr = INADDR_ANY;  //use this current IP address
    address.sin_port = htons( PORT );      //specify port to establish socket

    if( bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0 ){
        fprintf(stderr, "errno: %d\n", errno);
        perror("Error from perror");
        fprintf(stderr, "bind()\n");
        exit(EXIT_FAILURE);  //exit for now, MUST REMOVE LATER!
    }
    else
        printf("Bound an address to socket fd: %d\n", server_fd);

    //=====================================================
    //Step 4: Listening out for incoming socket connections
    //=====================================================
    if( listen(server_fd, BACKLOG) ){
        fprintf(stderr, "errno: %d\n", errno);
        perror("Error from perror");
        fprintf(stderr, "listen()\n");
        exit(EXIT_FAILURE);  //exit for now, MUST REMOVE LATER!
    }
    else
        printf("Listening for incoming connections...\n");

    //======================================
    //Step 5: Connect to pending connections
    //======================================
    while(1){
        //if(user array not full)
            if( (client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen )) < 0 ){
                fprintf(stderr, "errno: %d\n", errno);
                perror("Error from perror");
                fprintf(stderr, "accept()\n");
                exit(EXIT_FAILURE);  //exit for now, MUST REMOVE LATER!
            }
            else{
                printf("Connection established -> socket fd: %d\n", client_fd);
                numConnections++;   //need to decrement this when a user disconnects
                
                //TODO: create new USER struct and populate fields
                USER *u;
                //addUser(&u);    //may need slightly custom data structure to remove the correct user from the list on disconnect
                
                //send welcome message to new client
                send(client_fd, "Hello client!", strlen("Hello client!"), 0);
                sleep(2);
                send(client_fd, "yo", strlen("yo"), 0);
            }
        //else give some sort of error message

    }
    
    return 0;
}

//broadcast msg to all users in userlist
void broadcast(USER *userlist, char *msg){

}

//direct message from one user to another
void directMessage(USER *userlist, char *nickname){
    //need to include sender's name/sockfd in order to tell them if the message couldn't be sent (typo in nickname etc.)

}

//vote kick a user (might be better being implemented in main?)
void votekick(){

}