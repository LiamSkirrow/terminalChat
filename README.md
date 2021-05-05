*Note: Until development is complete, this README is being used as a development journal/note-to-self file*

# Spec
* a simple chat client that runs in the terminal
* capable of hosting both large and small chat rooms
* minimalist, simple, easy to use
## Features:
  * terminalchat can be run as a standalone executable where a default IP is connected to, or an optional flag is included where
    a custom IP can be specified to connect to so that the server can be hosted wherever the user chooses. 
  * ability to set/reset nickname -> all nicknames must be unique (no duplicates)
  * commands that show the total number of connections, show current socket IDs and corresponding nicknames
    when a user enters the command to see these stats, they are the only one that should receive this output -> the server
    must send this data to their socket only
  * send DMs to individual users, able to send messages based on nicknames
  * current socket connections can vote to kick a given user out (requires either 1/2 or 2/3 majority, must choose)
  * messages should be timestamped
  * curse word parsing, somewhere a list of censored words should be stored and every time a word in that list is
    mentioned it's replaced (client side) by the correct number of '*' chars. This censored word list should be easily
    editable
  * ability to write chat messages to an output LOG file (log file is updated everytime the chat window is updated)
  * ??? ability for a user to create a new server, maybe add password access
  * ??? read receipts


# General Notes
* **_include a makefile?_** 
* C strings can only be so long as they have predetermined length, show the buffer length somewhere in the UI... Maybe update
  how many chars are remaining as the user types
* _extend into an IRC client, or possibly an IRC bot?_


# UI Notes
- using ncurses for the client UI for terminalchat
- include a menu bar at the bottom showing the ctrl+? key combinations available, similarly to vim.


# Bugs 
* none so far :)


## Compile with 
gcc -D_GNU_SOURCE -D_DEFAULT_SOURCE -o client chat.c -lncurses -ltinfo

NOTE: make a makefile at some point 
