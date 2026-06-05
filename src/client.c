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

int main (int argc, char *argv[]) {

    // checks for correct number of arguments
    if (argc != 3) {
        printf("Error: IP and username not provided.");
        exit(1);
    }

    // sets up server socket
    int serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd == -1) {
        perror("Socket not made");
        exit(1);
    }

    // server address struct
    struct sockaddr_in serverAddress;

    serverAddress.sin_family = AF_INET;     // IPv4
    serverAddress.sin_port = htons(8080);   // converts 8080 to network label for port

    inet_pton(AF_INET, argv[1], &serverAddress.sin_addr);;     // accepts data from this address

    // connecting to theserver
    int connectResult = connect(serverFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (connectResult == -1) {
        perror("Connection failed");
        exit(1);
    }

    // checking valid username and building connect message
    char buffer[HEADER_SIZE + 65535];
    if (strlen(argv[2]) > 16) {
        printf("Username must be 16 characters or less\n");
        exit(1);
    }

    int messageLength = buildMessage(CONNECT, (uint16_t) strlen(argv[2]), argv[2], buffer);

    int sendResult = send(serverFd, buffer, messageLength, 0);
    if (sendResult == -1) {
        perror("Send failed.");
        exit(1);
    }

    // checking if ACK received from the server
    int recvResult = recv(serverFd, buffer, HEADER_SIZE, 0);

    if (recvResult <= 0) {
        perror("Receiving failed.");
        exit(1);
    }

    struct ParsedMessage parsedMessage = parseMessage(buffer);
    if (parsedMessage.messageType != ACK) {
        printf("Acknowledgment not received.");
        exit(1);
    }

    // main loop
    while (1) {
        fd_set readFds;
        FD_ZERO(&readFds);  // clears the fd_set - all zeroes
        FD_SET(serverFd, &readFds);  // adds the serverSocketFd
        FD_SET(STDIN_FILENO, &readFds);

        int maxFd = serverFd;
        int selectResult = select(maxFd + 1, &readFds, NULL, NULL, NULL);
        if (selectResult == -1) {
            perror("Select failed");
            exit(1);
        }

        if (FD_ISSET(STDIN_FILENO, &readFds)) {
            char input[65535];
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0;

            if (input[0] == '/') {
                char *inputCopy = strdup(input);
                char *messageType = strsep(&inputCopy, " ");

                if (strcmp(messageType, "/join") == 0) {
                    char *joinValue = "";
                    joinValue = strsep(&inputCopy, " ");

                    messageLength = buildMessage(JOIN, (uint16_t) strlen(joinValue), joinValue, buffer);
                }else if (strcmp(messageType, "/create") == 0) {
                    messageLength = buildMessage(CREATE, 0, NULL, buffer);
                }
            }
            else {
                messageLength = buildMessage(MSG, (uint16_t) strlen(input), input, buffer);

                sendResult = send(serverFd, buffer, messageLength, 0);
                
                if (sendResult == -1) {
                    perror("Error when sending.");
                    exit(1);
                }
            }
            
        }
        if (FD_ISSET(serverFd, &readFds)) {
            // getting header from server
            int recvResult = recv(serverFd, buffer, HEADER_SIZE, 0);
            
            if (recvResult <= 0) {
                perror("Receiving failed.");
                exit(1);
            }

            uint16_t lengthOfBytes;
            memcpy(&lengthOfBytes, buffer + 3, sizeof(uint16_t));

            int length = recv(serverFd, buffer + HEADER_SIZE, lengthOfBytes, 0);
            struct ParsedMessage parsedMessage = parseMessage(buffer);
            if (parsedMessage.messageType == MSG) {
                printf("%.*s\n", parsedMessage.lengthOfMessage, parsedMessage.payload);
            }

        }
    } 

}