# **ZMP - Zubair Messaging Protocol**

The ZMP is a TCP-based custom protocol that allows for multiclient connection and messaging over a server. It has its own verification system using a Fletcher-16 checksum algorithm and has multiple messaging types.

## **Features**

The ZMP consists of the following features;
1. Multiclient connections across a server over a command line interface (CLI).
2. Acknowledgements between server and clients.
3. Custom packet with a message type, message length, checksum and payload.
4. Multiplexing to allow server to seemingly respond to multiple clients at once.
5. Graceful server shut down.
6. Room Creation & Joining.
7. Room based message sending.
8. Maximum of 10 rooms including starting/global room.

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

After this anything typed in and entered will be considered as a message that is sent to other clients except when the beginning character is / in which case that is considered as the client sending a command to the server. Currently there are only three main commands that a client can give to a server: '/create', '/join' and '/exit'. '/create' will create a new room (only a maximum of 10 rooms including the starting room) and '/join <roomID>' will allow the client to join a specific room. The server will inform the client if the room has been created or not and whether they have joined the room respectively for each command. The '/exit' command will make the client exit the connection.

```
/create
/join 1
/exit
```

To stop the program then run Ctrl+C in the server terminal.


## **Limitations and Improvements**

1. Currently there are some bugs with how rooms operate such as when more than 10 are exceeded.
2. The protocol does not have any system for checking people that join. It doesnt validate the users by checking their password before allowing them access to the server.
3. The messages are not encrypted as they are sent across the server.
4. Rooms are stil quite basic as they only have an ID and no name. They are also always active after they are created rather than being destroyed once empty.
5. Clients still dont know who is sending what message as just he message is sent with no indication of who sent it.
