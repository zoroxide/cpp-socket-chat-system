#include "Client.hpp"

// Constructor: Initialize socket and server address
Client::Client() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cout << "Socket creation error" << std::endl;
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
}

// Connect to the server
bool Client::connectToServer(const std::string& ip_address) {
    if (inet_pton(AF_INET, ip_address.c_str(), &serv_addr.sin_addr) <= 0) {
        std::cout << "Invalid address / Address not supported" << std::endl;
        return false;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Connection failed" << std::endl;
        return false;
    }

    return true;
}

// Send a message to the server
void Client::sendMessage(const std::string& message) {
    send(sock, message.c_str(), message.size(), 0);
}

// Listen for messages from the server
void Client::listenForMessages() {
    char buffer[1024] = {0};
    while (true) {
        ssize_t bytes_read = read(sock, buffer, sizeof(buffer) - 1);
        if (bytes_read <= 0) {
            std::cout << "Disconnected from server" << std::endl;
            break;
        }

        buffer[bytes_read] = '\0'; // Null-terminate the string
        std::cout << "New message: " << buffer << std::endl;
    }
}

// Close the connection
void Client::closeConnection() {
    close(sock);
}
