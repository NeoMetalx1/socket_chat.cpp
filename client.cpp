#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int main()
{
    // creating socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    const char* serverIP = "ip_server_there";
    inet_pton(AF_INET, serverIP, &serverAddress.sin_addr);

    // sending connection request
    connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    // sending data
    std::string nickname;
    std::string message;
    std::cout << "Enter nickname: ";
    std::getline(std::cin, nickname);
    std::cout << std::endl;

    while(true){
        std::cout << "Enter message: ";
        std::getline(std::cin, message);
        std::cout << std::endl;
        std::string full_message = "<" + nickname + "> " + message;

        send(clientSocket, full_message.c_str(), full_message.size(), 0); 
        
        if(message == "exit"){
            break;
        }
    }

    close(clientSocket);
    return 0;
}
