//
// Created by Valery on 15.12.2024.
//

#include "transmission.h"
#include <iostream>
#include <string>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <windows.h>
#include <winsock2.h>
#include <thread>
#include "../metrics/metrics.h"

#pragma comment(lib, "ws2_32.lib")

void handle_client(SOCKET client_socket) {
    const std::string GET_LOAD_CPU = "get_cpu_load";
    char buffer[1024];

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_read <= 0) {
            std::cerr << "Client disconnected or error reading from socket." << std::endl;
            break;
        }

        std::string request(buffer);
        std::cout << "Received request: " << request << std::endl;

        if (request.find(GET_LOAD_CPU) != std::string::npos) {
            double cpu_load = getCPUUsage();
            std::ostringstream response;
            response << cpu_load;
            send(client_socket, response.str().c_str(), response.str().size(), 0);
        } else {
            std::string error_response = "Unknown command\n";
            send(client_socket, error_response.c_str(), error_response.size(), 0);
        }
    }

    closesocket(client_socket);
}

void setup_server_socket(SOCKET& server_socket, sockaddr_in& server_address, int port) {
    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed. Error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
        std::cerr << "Bind failed. Error: " << WSAGetLastError() << std::endl;
        closesocket(server_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed. Error: " << WSAGetLastError() << std::endl;
        closesocket(server_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is listening on port " << port << "..." << std::endl;
}

int transmissionMain() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    const int PORT = 8080;
    SOCKET server_socket;
    sockaddr_in server_address{};

    setup_server_socket(server_socket, server_address, PORT);

    while (true) {
        sockaddr_in client_address;
        int client_len = sizeof(client_address); // Заменяем socklen_t на int
        SOCKET client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_len);
        if (client_socket == INVALID_SOCKET) {
            std::cerr << "Failed to accept client connection. Error: " << WSAGetLastError() << std::endl;
            continue;
        }

        std::cout << "Client connected." << std::endl;

        std::thread client_thread(handle_client, client_socket);
        client_thread.detach();
    }

    closesocket(server_socket);
    WSACleanup();
}
