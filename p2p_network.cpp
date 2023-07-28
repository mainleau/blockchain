// p2p_network.cpp

#include "p2p_network.h"
#include <iostream>
#include <algorithm>

void P2PNetwork::startServer() {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error binding socket" << std::endl;
        return;
    }

    if (listen(sockfd, 5) < 0) {
        std::cerr << "Error listening on socket" << std::endl;
        return;
    }

    std::cout << "P2P Server started, listening for incoming connections..." << std::endl;

    while (true) {
        int clientSockfd = accept(sockfd, NULL, NULL);
        if (clientSockfd < 0) {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }

        // Create a new thread to handle the connection with the client
        std::thread connectionThread(&P2PNetwork::handleConnection, this, clientSockfd);
        connectionThread.detach(); // Detach the thread to let it run independently
    }
}

void P2PNetwork::connectToPeer(const std::string& ipAddress, int port) {
    int peerSockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (peerSockfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }

    sockaddr_in peerAddr;
    peerAddr.sin_family = AF_INET;
    peerAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, ipAddress.c_str(), &peerAddr.sin_addr);

    if (connect(peerSockfd, (struct sockaddr*)&peerAddr, sizeof(peerAddr)) < 0) {
        std::cerr << "Error connecting to peer" << std::endl;
        close(peerSockfd);
        return;
    }

    // Add the new peer socket to the list of connected sockets
    std::lock_guard<std::mutex> lock(socketMutex);
    connectedSockets.push_back(peerSockfd);

    std::cout << "Connected to peer at " << ipAddress << std::endl;
}

void P2PNetwork::handleConnection(int clientSockfd) {
    std::cout << "New connection established" << std::endl;

    while (true) {
        char buffer[1024] = {0};
        int bytesReceived = recv(clientSockfd, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            // Error or connection closed by the client, remove the socket from the list
            std::lock_guard<std::mutex> lock(socketMutex);
            auto it = std::find(connectedSockets.begin(), connectedSockets.end(), clientSockfd);
            if (it != connectedSockets.end()) {
                connectedSockets.erase(it);
            }

            close(clientSockfd);
            std::cout << "Connection closed" << std::endl;
            return;
        }

        std::cout << "Message received: " << buffer << std::endl;

        // Broadcast the received message to all connected peers (except the sender)
        broadcast(buffer, clientSockfd);
    }
}

void P2PNetwork::broadcast(const std::string& message, int senderSockfd) {
    std::lock_guard<std::mutex> lock(socketMutex);
    for (int peerSockfd : connectedSockets) {
        if (peerSockfd != senderSockfd) {
            send(peerSockfd, message.c_str(), message.size(), 0);
        }
    }
}