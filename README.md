# **ZMP - Zubair Messaging Protocol**

The ZMP is a TCP-based custom protocol that allows for multiclient connection and messaging over a server. It has its own verification system using a Fletcher-16 checksum algorithm and has multiple messaging types.

## **Features**

The ZMP consists of the following features;
1. Multiclient connections across a server over a command line interface (CLI).
2. Acknowledgements between server and clients.
3. Custom packet with a message type, message length, checksum and payload.
4. Graceful server shut down

## **Requirements**

- Compiler: GCC (or clang) with C99 version of the C language (or any other version after that).  
- Operating System: Linux or MacOS (since the program uses the POSIX APIs which are only available on UNIX based systems).  
- No external libraries need to be imported.  
- Build Tool: make file (provided on github)


## **Building and Usage**

Run the make file using the "make" command. This creates executable binaries for the server and clients. Then run "./server" which will run the server and start it up. After the server has been made, create as many new terminals as you want and for each one run "./client <ipaddress> <username>". This will get the clients connected to the server and messages can then be sent between them.

```
make
./server
./client 127.0.0.1 Zubair
./client 127.0.0.1 Sam
```
To stop the program then run Ctrl+C in the server terminal.


## **Limitations and Improvements**

1. The protocol currently sends messages from one client to all other clients. An improvement to this would be to add chat rooms so that only those within a certain room can see the messages.
2. The protocol does not have any system for checking people that join. It doesnt validate the users by checking their password before allowing them access to the server.
3. The messages are not encrypted as they are sent across the server.
