//
// Created by Valery on 15.12.2024.
//

#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include <winsock2.h>

void setup_server_socket(SOCKET& server_socket, sockaddr_in& server_address, int port);
void handle_client(SOCKET client_socket);
int transmissionMain();

#endif //TRANSMISSION_H
