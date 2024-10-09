#pragma once

#include <bits/stdc++.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080

class Client {
private:
    int sock;
    struct sockaddr_in serv_addr;

public:
    Client();
    bool connectToServer(const std::string& ip_address);
    void sendMessage(const std::string& message);
    void listenForMessages();
    void closeConnection();
};
