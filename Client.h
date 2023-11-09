#pragma once

#include <iostream>
#include <unistd.h>
#include <string.h>
#include <limits>
#include <sys/socket.h>
#include <arpa/inet.h>


#define MESSAGE_LENGTH 1024
#define PORT 7777
#define CONNECTED 1

using std::string;

void ConnectServer(bool& status);
void Register(int& __fd, const std::string& username, const string& password); 
void Login();
void Logout();
void DeleteUser();
void CreateMessage();
void ShowMessage();
void LoginMenu(bool& status);
void UserMenu();

int socket_file_descriptor;
int connection;
struct sockaddr_in serveraddress, client;
char clientRequest[MESSAGE_LENGTH];
char serverResponse[MESSAGE_LENGTH];
static bool Terminator = false;

void Register(int& __fd, const std::string& username, const string& password)
{
    std::cout << "Registering user!\n";
    bzero(clientRequest, sizeof(clientRequest));    
    strncpy(clientRequest, "001", sizeof(clientRequest));
    ssize_t bytes = write(socket_file_descriptor, clientRequest, sizeof(clientRequest));
    if (bytes >= 0)
        {
               std::cout << "Data was sent successfuly!\n";
        }
    
    bzero(serverResponse, sizeof(serverResponse));
    read(__fd, serverResponse, sizeof(serverResponse));
    std::cout << serverResponse << std::endl;
    if (strncmp("0010", serverResponse, 3) == 0)
    {
        std::cout << "User failed to Register!\n";                        
    }
    /*
    strcpy(clientRequest, username.c_str()); 
    std::cout << "Checking username...\n";

    ssize_t bytes = write(__fd, clientRequest, sizeof(clientRequest));
    if (bytes >= 0)
    {
        std::cout << "Data was sent successfuly!\n";
    }
    bzero(serverResponse, sizeof(serverResponse));
    read(__fd, serverResponse, sizeof(serverResponse));

    std::cout << "Data received from server: " << serverResponse << std::endl;
    */
}

void LoginMenu(bool& status)
{
    string username, password;
    int menuOperator = 0;
    std::cout << "Welcome to CLI Chat, please choose your option:\n";
    std::cout << "\t0 - Exit\n"
        << "\t1 - Register\n"
        << "\t2 - Login\n"
        << "\t3 - Forgot Password\n";  
      
    while (!(std::cin >> menuOperator))
    {

        std::cin.clear();

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Invalid input! Please try again with valid input: ";
    }

    switch (menuOperator)
    {
    case 0:
        std::cout << "Closing application!\n";
        Terminator = true;
        break;
    case 1:        
        Register(socket_file_descriptor, username, password);
        break;
    case 2:         
        
        break;
    default:
        std::cout << "Wrong statement!\n";
        break;
    }
}

void ConnectServer(bool& status)
{
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptor == -1)
    {
        std::cout << "Failed to create socket!\n";
        exit(1);
    }

    serveraddress.sin_family = AF_INET;
    serveraddress.sin_addr.s_addr = inet_addr("172.16.8.1");
    serveraddress.sin_port = htons(PORT);
    
    connection = connect(socket_file_descriptor, (struct sockaddr*)& serveraddress, sizeof(serveraddress));
    if (connection == -1)
    {
        std::cout << "Failed to establish connectivity!\n";
        exit(1);
    }
    else 
    {
        std::cout << "Connection with " << inet_ntoa(serveraddress.sin_addr)<< " has been established!\n";
    }
    bzero(serverResponse, sizeof(serverResponse));
    read(socket_file_descriptor, serverResponse, sizeof(serverResponse));
    std::cout << serverResponse << std::endl;
    bzero(serverResponse, sizeof(serverResponse));
    while (!Terminator)
    {
        LoginMenu(status);
    }
    close(socket_file_descriptor);

}