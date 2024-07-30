#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <fstream>
using namespace std;

#pragma comment(lib, "Ws2_32.lib")

#define PORT 8080
#define BACKLOG 10

void handle(SOCKET clientSocket);

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int addr_size;

   
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket" << std::endl;
        WSACleanup();
        return -1;
    }


    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Error binding socket" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

   
    if (listen(serverSocket, BACKLOG) == SOCKET_ERROR) {
        std::cerr << "Error listening on socket" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    while (true) {
        addr_size = sizeof(clientAddr);
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addr_size);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }

        std::thread(handleClient, clientSocket).detach();
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}

void handleClient(SOCKET clientSocket) {
    char buffer[1024];
    recv(clientSocket, buffer, sizeof(buffer), 0);

    std::string request(buffer);
    std::cout << "Received request:\n" << request << std::endl;

   
    std::istringstream requestStream(request);
    std::string method, path, version;
    requestStream >> method >> path >> version;

    if (path == "/") {
        path = "/index.html";
    }

    std::ifstream file("www" + path);
    if (file.is_open()) {
        std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
        std::string line;
        while (getline(file, line)) {
            response += line + "\n";
        }
        file.close();
        send(clientSocket, response.c_str(), response.size(), 0);
    }
    else {
        std::string notFoundResponse = "HTTP/1.1 404 NOT FOUND\r\nContent-Type: text/html\r\n\r\n<html><body><h1>404 Not Found</h1></body></html>";
        send(clientSocket, notFoundResponse.c_str(), notFoundResponse.size(), 0);
    }

    closesocket(clientSocket);
}
