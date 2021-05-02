/* a test program mainly to experiment with ncurses */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <ncurses.h> 


/* TODO: to get minimal fuctionality
>> have a look into <form.h>, this may include the solution to using the arrow keys and backspace key
   in the input text field
- possibly modes similar to vim
    - press 'i' to enter text into the input form field
    - press 'esc' to enter scroll-mode, where 'j' & 'k' can be used to scroll through the chat history
    - alternatively just use some combination of ctrl+KEY to enter scroll/insert modes
*/


int main(int argc, char **argv){
    char buf[100];
    int ind = 0;
    initscr();
    raw();

    int ymax, xmax;
    getmaxyx(stdscr, ymax, xmax);

    //WINDOW *win_write = newwin(ymax/4, xmax/4, 0, xmax/4);
    //WINDOW *win_read  = newwin(ymax/4, xmax/4, ymax/2, xmax/4);

    WINDOW *win_read = newwin(ymax-6, xmax, 0, 0);
    WINDOW *win_write  = newwin(5, xmax, ymax-5, 0);

    //enable arrow keys/backspace etc. 
    keypad(win_write, TRUE);


    box(win_write, 0, 0);
    box(win_read, 0, 0);
    //mvwprintw(win, 0, 2, "File");
    //wmove(win, 1, 1);

    wrefresh(win_write);
    wrefresh(win_read);

    //might want to declare these at the top...
    int pid = fork();
    int count; int ch;
    int x = 0;
    int y = 0;


    for(count = 0; count < 10; count++){
        if(pid == 0){  //child process
            //ch = wgetch(win_write);
            wgetnstr(win_write, buf, sizeof(buf));

            //decode user input, TODO: note: this will probably go if <form.h> works
            /*switch( ch ){
                case KEY_LEFT:
                    x--;
                    break;
                case KEY_RIGHT:
                    x++;
                    break;
                
                //TODO: other key inputs including ctrl+key inputs

                //wgetch(win_write);
                //wgetstr(win_write, buf);

            } */
            wmove(win_write, y, ++x);
            wrefresh(win_write);
            wrefresh(win_read);
        }
        else{   //parent process
            mvwprintw(win_read, ++y, 1, "%d", count);
            wrefresh(win_read);
            wrefresh(win_write);
            sleep(1);
        }
    }
    
    if(pid != 0) {
        //parent kill child process
        kill(pid, SIGTERM);
    }
    else{
        //child kill parent process
        kill(getppid(), SIGTERM);
    }

    endwin();
    return 0;
}