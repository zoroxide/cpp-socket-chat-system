#include "../../include/client/Client.hpp"

int main() {
    Client client;

    // Connect to the server
    if (!client.connectToServer("127.0.0.1")) {
        return -1;
    }

    // Start a thread to listen for messages
    std::thread listener(&Client::listenForMessages, &client);
    listener.detach(); // Detach the listener thread

    // Main loop for sending messages
    std::string message;
    while (true) {
        std::cout << "Enter message: ";
        std::getline(std::cin, message);
        if (message == "exit") break; // Exit condition
        client.sendMessage(message);
    }

    // Close the connection before exiting
    client.closeConnection();
    return 0;
}
