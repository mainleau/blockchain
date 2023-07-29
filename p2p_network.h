// p2p_network.h

#ifndef P2P_NETWORK_H
#define P2P_NETWORK_H

#include <iostream>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <mutex>
#include "blockchain.h"

class P2PNetwork {
    public:
        void startServer(int port);
        void connectToPeer(const std::string& ipAddress, int port);
        void broadcast(const std::string& message, int senderSockfd = -1);
        void listenForConnections();
    private:
        void handleConnection(int clientSockfd);
        int sockfd;

        std::vector<int> connectedSockets;
        std::mutex socketMutex;
};

#endif // P2P_NETWORK_H
