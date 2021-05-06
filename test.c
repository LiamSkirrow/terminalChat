/* a test program mainly to experiment with ncurses */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <ncurses.h> 
#include <form.h>

#define CHAT_HISTORY_FIELD 0
#define MESSAGE_FIELD 1


/* TODO: to get minimal fuctionality
>> have a look into <form.h>, this may include the solution to using the arrow keys and backspace key
   in the input text field
- implement similar modes to vim:
    - press 'i' to enter text into the input form field
    - press 'esc' to enter scroll-mode, where 'j' & 'k' can be used to scroll through the chat history
    - alternatively just use some combination of ctrl+KEY to enter scroll/insert modes
*/


int main(int argc, char **argv){
    char buf[100];
    int character;
    int ind = 0;
  
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    
    FORM *form;
    FIELD *fields[3];
    WINDOW *win_read, *win_write;

    int ymax, xmax;
    getmaxyx(stdscr, ymax, xmax);

    //initialise windows set size/location on screen
    win_read  = newwin(ymax-6, xmax, 0, 0);
    win_write = newwin(5, xmax, ymax-5, 0);

    box(win_write, 0, 0);
    box(win_read, 0, 0);
    
    //TODO: no offscreen buffers just yet
    //initialise the fields
    //fields[0] = new_field(ymax-6, xmax, 0, 0, 0, 0);
	//fields[1] = new_field(5, xmax, ymax-5, 0, 0, 0);

    fields[CHAT_HISTORY_FIELD] = new_field(ymax-8, xmax-3, 1, 1, 0, 0);   //read-only form to display chat history
	fields[MESSAGE_FIELD] = new_field(3, xmax-2, ymax-4, 1, 0, 0);   //read/write form to write messages
    fields[3] = NULL;

    //set field options
    set_field_back(fields[CHAT_HISTORY_FIELD], 0);  //underline the field
    field_opts_off(fields[CHAT_HISTORY_FIELD], O_AUTOSKIP | O_ACTIVE);  	 //disable auto field advance, user write
    //set_field_opts(fields[0], O_AUTOSKIP);

    set_field_back(fields[MESSAGE_FIELD], 0);  //underline the field
    field_opts_off(fields[MESSAGE_FIELD], O_AUTOSKIP);  	 //disable auto field advance
    //set_field_opts(fields[1], O_AUTOSKIP);

    //create the form and post it
    form = new_form(fields);
    post_form(form);
	refresh();

	wrefresh(win_write);
	wrefresh(win_read);


    

    //might want to declare these at the top...
    int pid = fork();
    int count; int ch;
    int x = 0;
    int y = 0;
    int enterPressed;

    for(count = 0; count < 2; count++){
        if(pid == 0){  //child process, block on getting user input
            while( (character = getch()) != KEY_F(1) ){
                enterPressed = formdriver(form, character);
                
                if(enterPressed){
                    //check if the 'enter' key has been pressed
                    //if so, call field_buffer() and get the user input string
                    //if string is empty (false press) don't send anything to server
                    //otherwise, add metadata to string and send it to server
                }
            }
        }
        else{   //parent process, display messages from server to display
            //TODO: need a way to keep track of the current buffer number to write to
            set_field_buffer(fields[CHAT_HISTORY_FIELD], count, "test");
            
            //refresh();
            

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

    //free all the allocated memory
    unpost_form(form);
	free_form(form);
	free_field(fields[0]);
	free_field(fields[1]);
	delwin(win_read);
	delwin(win_write);
    endwin();

    return 0;
}

int formdriver(FORM *form, int character){
    int enterPressed = 0;

    switch(character){
        case KEY_LEFT:  //move cursor left one char
            form_driver(form, REQ_PREV_CHAR);
            break; 
        
        case KEY_RIGHT: //move cursor right one char
            form_driver(form, REQ_NEXT_CHAR);
            break;

        case KEY_BACKSPACE: //move cursor right one char
            form_driver(form, REQ_DEL_PREV);
            break;

        case KEY_ENTER:     //TODO: check if this is the correct 'enter' name
            enterPressed = 1;
            break;

        default:        //default case -> print out user input
            form_driver(form, character);
            break;
    }

    return enterPressed;
}