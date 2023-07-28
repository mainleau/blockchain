// blockchain_node.h

#ifndef BLOCKCHAIN_NODE_H
#define BLOCKCHAIN_NODE_H

#include <vector>
#include <string>
#include "blockchain.h"
#include "p2p_network.h"

class BlockchainNode {
    public:
        BlockchainNode(int port);
        void start();

        void connectToInitialPeers();
        void connectToPeer(const std::string& ipAddress, int port);
        void handleNewBlock(const Block& block);
    private:
        Blockchain blockchain;
        P2PNetwork p2pNetwork;
};

#endif // BLOCKCHAIN_NODE_H
