/* Terminalchat: this is the client-side code */

//TODO:
//- include -help commandline option to show available flags

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h> 

#include <sys/socket.h>
//#include "chat.h"

#define PORT 8080
#define MAX_MESSAGE_LENGTH 100

int main(int argc, char **argv){
    int sock;           //placeholder socket fd
    int ymax, xmax;     //ncurses window dimensions
    struct sockaddr_in serv_addr;   //socket data struct
    char buf[13];                   //buffer to store received message data
    char messageInput[50];          //message data to send to the server

    //check to see if the user has included the -r flag to specify their own destination IP address
    //otherwise use default IP address (localhost: 127.0.0.1)


    /* === ncurses setup START === */
    initscr();
    cbreak();    
    curs_set(0);

    getmaxyx(stdscr, ymax, xmax);

    WINDOW *win = newwin(ymax/2, xmax/2, ymax/4, xmax/4);
    box(win, 0, 0);

    mvwprintw(win, 0, 2, "File");
    move(0,0);

    wrefresh(win);
    /* === ncurses setup END === */


    //TODO: all printfs will have to be replaced with printw()'s ???


    //establish a socket connection to the server
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "errno: %d\n", errno);
        perror("Error from perror");
        fprintf(stderr, "socket()\n");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;    //specify IPv4
    serv_addr.sin_port = htons(PORT);  //specify port number to connect to

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  //TODO: replace hardcoded IP with either default or user-specified
    {
        fprintf(stderr, "errno: %d\n", errno);
        perror("Error from perror");
        fprintf(stderr, "inet_pton()\n");
        exit(EXIT_FAILURE);
    }

    //establish socket connection
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        fprintf(stderr, "errno: %d\n", errno);
        perror("Error from perror");
        fprintf(stderr, "connect()\n");
        exit(EXIT_FAILURE);
    }

    //main control loop
    while(1){
        //block on read() until a new message is available for display
        read(sock, buf, MAX_MESSAGE_LENGTH);
        printw("Message received");
        wrefresh(win);
        memset(buf, '\0', sizeof(buf));            //clear buf to avoid re-printing old messages

        //fgets(messageInput, 50, stdin);
        //printf("User input: %s\n", messageInput);


        //endwin();
    }

    endwin();

    return EXIT_SUCCESS;
}