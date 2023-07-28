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

const int PORT = 8080;

class P2PNetwork {
public:
    void startServer();
    void connectToPeer(const std::string& ipAddress);
    void broadcast(const std::string& message, int senderSockfd = -1);
private:
    void handleConnection(int clientSockfd);

    int sockfd;
    std::vector<int> connectedSockets;
    std::mutex socketMutex;
};

#endif // P2P_NETWORK_H
