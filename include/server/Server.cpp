#include "Server.hpp"

// Constructor: Initialize socket
Server::Server() {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Socket creation failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Binding failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, CONNECTIONS_BACKLOG) < 0) {
        std::cerr << "Listening failed" << std::endl;
        exit(EXIT_FAILURE);
    }
}

// Broadcast message to all connected clients
void Server::broadcast_message(const std::string& message) {
    std::lock_guard<std::mutex> lock(mtx);
    for (int client_socket : client_sockets) {
        send(client_socket, message.c_str(), message.size(), 0);
    }
}

// Handle each client connection
void Server::handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    while (true) {
        ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
        if (bytes_read <= 0) {
            break; // Client disconnected
        }

        buffer[bytes_read] = '\0'; // Null-terminate the string
        std::string message(buffer);
        std::cout << "Received: " << message << std::endl;

        // Broadcast the received message to all clients
        broadcast_message(message);
    }

    // Remove client from the list and close the socket
    {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = std::find(client_sockets.begin(), client_sockets.end(), client_socket);
        if (it != client_sockets.end()) {
            client_sockets.erase(it);
        }
    }
    close(client_socket);
}

// Run the server to accept clients
void Server::run(std::string host) {
    std::cout << "Server running on " << host << ":" << PORT << std::endl;

    while (true) {
        int client_socket = accept(server_socket, nullptr, nullptr);
        if (client_socket < 0) {
            std::cerr << "Accept failed" << std::endl;
            continue;
        }

        std::cout << "New client connected" << std::endl;

        {
            std::lock_guard<std::mutex> lock(mtx);
            client_sockets.push_back(client_socket);
        }

        std::thread client_thread(&Server::handle_client, this, client_socket);
        client_thread.detach(); // Detach the thread to allow multiple clients
    }
}
