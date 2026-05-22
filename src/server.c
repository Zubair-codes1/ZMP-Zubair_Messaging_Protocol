#include <stdint.h>       // unsigned ints  
#include <stdbool.h>      // boolean access
#include <sys/socket.h>   // socket(), bind(), listen(), accept(), connect()
#include <netinet/in.h>   // sockaddr_in, htons(), htonl()
#include <arpa/inet.h>    // inet_pton(), inet_ntop()
#include <unistd.h>       // close(), read(), write()
#include <string.h>       // memset(), memcpy()
#include "protocol.h"
#include <stdlib.h>    // for exit()
#include <stdio.h>     // for perror()

// holds information about each client
struct Client {
    int socketfd;           // OS can return -1 hence regular int
    char username[16];      // 16 bytes for username
    bool isInUse;
    struct sockaddr_in address;
};

// max clients and backlog size constants
#define MAX_CLIENTS 10
#define BACKLOG_SIZE 10

// creating a list of client structs
struct Client clients[MAX_CLIENTS];

// forward declarations for initialiseServer and runServer
int initialiseServer();
void runServer(int serverSocketFd);

// main - puts together all the server code
int main () {
    memset(clients, 0, sizeof(clients));
    int serverSocketFd = initialiseServer();
    runServer(serverSocketFd);
    return 0;
}

/**
 * Initialises the server - binding to port and listening for clients
 * @author Zubair Abdul Matin
 */
int initialiseServer() {
    int serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocketFd == -1) {
        perror("Socket not created");
        exit(1);
    }

    int opt = 1;
    int sockopt = setsockopt(serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (sockopt == -1) {
        perror("Socket options not working");
        exit(1);
    }

    // server address struct
    struct sockaddr_in serverAddress;

    serverAddress.sin_family = AF_INET;     // IPv4
    serverAddress.sin_port = htons(8080);   // converts 8080 to network label for port
    serverAddress.sin_addr.s_addr = INADDR_ANY;     // accepts data from any interface type from OS

    // binds the server socket file descriptor to the 8080 port
    int bindResult = bind(serverSocketFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    if (bindResult == -1) {
        perror("Binding failed");
        exit(1);
    }

    int listenResult = listen(serverSocketFd, BACKLOG_SIZE);
    if (listenResult == -1) {
        perror("Listening failed");
        exit(1);
    }

    return serverSocketFd;
}

// accepting, selecting and receiving messages
void runServer(int serverSocketFd)
{
    // setting up the fds to listen to
    while(1) {
        fd_set read_fds;
        FD_ZERO(&read_fds);  // clears the fd_set - all zeroes
        FD_SET(serverSocketFd, &read_fds);  // adds the serverSocketFd
        
        int max_fd = serverSocketFd;

        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i].isInUse) {
                FD_SET(clients[i].socketfd, &read_fds);

                if (clients[i].socketfd > max_fd) {
                    max_fd = clients[i].socketfd;
                }
            }
            
        }

        // selects the first connection from which a message is being sent
        int selectResult = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        if (selectResult == -1) {
            perror("Selection failed!");
            exit(1);
        }

        // accepting client requests
        if (FD_ISSET(serverSocketFd, &read_fds)) {
            struct sockaddr_in clientAddress;
            socklen_t clientLen = sizeof(clientAddress);
            int clientFd = accept(serverSocketFd, (struct sockaddr *)&clientAddress, &clientLen);

            int freeSlot = -1;
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (!clients[i].isInUse) {
                    freeSlot = i;
                    break;
                }
            }

            if (freeSlot == -1) {
                close(clientFd);
            }else {
                clients[freeSlot].socketfd == clientFd;
            }
        }

    }
    
}
