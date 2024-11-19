#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>

void handleClient(int clientSocket) {
    while (true) {
        char buffer[1024] = {0};
        int text_receive = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (text_receive <= 0) {
            std::cout << "Клиент отключился." << std::endl;
            close(clientSocket);
            break;
        }

        std::cout << buffer << std::endl;

        if (strcmp(buffer, "exit") == 0) {
            std::cout << "Один пользователь отключился." << std::endl;
            close(clientSocket);
            break;
        }
    }
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    listen(serverSocket, 3);

    std::cout << "Сервер запущен и ожидает подключений..." << std::endl;

    while (true) {
        sockaddr_in clientAddress;
        socklen_t clientAddressLen = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLen);

        if (clientSocket < 0) {
            std::cerr << "Ошибка подключения клиента." << std::endl;
            continue;
        }

        std::cout << "Клиент подключился." << std::endl;

        if (fork() == 0) {  
            close(serverSocket); 
            handleClient(clientSocket);
            return 0; 
        }

        close(clientSocket);
    }

    close(serverSocket);
    return 0;
//отправлять то что приняли (recv() => send())
}