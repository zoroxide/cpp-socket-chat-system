#pragma once

#include <bits/stdc++.h>
#include <netinet/in.h>
#include <sys/socket.h>


typedef enum {
    LISTEN_SUCCESS,
    LISTEN_FIELD,
    ACCEPT_SUCCESS,
    ACCEPT_FIELD,
    BIND_SUCCESS,
    BIND_FIELD
} State_t;

class Server {
private:
    #define HOST "127.0.0.1"
    #define PORT 8080
    #define CONNECTIONS_BACKLOG 10
    #define BUFFER_SIZE 1024

    int server_socket;
    std::vector<int> client_sockets;
    std::mutex mtx;

    void handle_client(int client_socket);
    void broadcast_message(const std::string& message);

public:
    Server();
    void run(std::string host = HOST);
};
