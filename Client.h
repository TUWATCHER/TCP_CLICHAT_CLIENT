#pragma once

#include <iostream>
#include <unistd.h>
#include <string.h>
#include <limits>
#include <sys/socket.h>
#include <arpa/inet.h>


#define MESSAGE_LENGTH 1024
#define PORT 7777
#define ERROR -1

void Connect(const char* address);

int socket_file_descriptor;
int connection, activity;
struct sockaddr_in serveraddress;
char serverResponse[MESSAGE_LENGTH];

void Connect(const char* address)
{
    // Creating socket
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptor == ERROR)
    {
        perror("Failed to create socket!");
        exit(EXIT_FAILURE);
    }

    // Assigning server address
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_addr.s_addr = inet_addr(address);
    serveraddress.sin_port = htons(PORT);
    
    // Connecting to server
    connection = connect(socket_file_descriptor, (struct sockaddr*)& serveraddress, sizeof(serveraddress));
    if (connection == ERROR)
    {
        perror("Failed to establish connectivity!");
        exit(EXIT_FAILURE);
    }

    else 
    {
        std::cout << "Connection with " << inet_ntoa(serveraddress.sin_addr) << " has been established!\n";
    }

    bzero(serverResponse, sizeof(serverResponse));
    recv(socket_file_descriptor, serverResponse, sizeof(serverResponse), 0);
    std::cout << serverResponse << std::endl;

    bzero(serverResponse, sizeof(serverResponse));

    while(true)
    {

        fd_set reads;
        FD_ZERO(&reads);
        FD_SET(socket_file_descriptor, &reads);
        FD_SET(0, &reads);

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 100000;

        activity = select(socket_file_descriptor+1, &reads, 0, 0, &timeout);
        if (activity < 0)
        {
            perror("Select() error!");
            exit(EXIT_FAILURE);            
        }

        if (FD_ISSET(socket_file_descriptor, &reads))        
        {
            char read[4096];
            int bytes_received = recv(socket_file_descriptor, read, 4096, 0);
            if (bytes_received < 1)
            {
                std::cout << "Connection closed by peer.\n";
                break;
            }

            std::cout << "Received ("<< bytes_received << " bytes): "
                        << read;
        }

        if(FD_ISSET(0, &reads))
        {
            char read[4096];
            std::cout << "Enter your message!\n";
            if (!fgets(read, 4096, stdin)) break;
            int bytes_sent = send(socket_file_descriptor, read, strlen(read), 0);
            std::cout << "Sent " << bytes_sent << " bytes\n";
        }
    }

    std::cout << "Closing socket...\n";
    close(socket_file_descriptor);

}