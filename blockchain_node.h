// blockchain_node.h

#ifndef BLOCKCHAIN_NODE_H
#define BLOCKCHAIN_NODE_H

#include <vector>
#include <string>
#include "blockchain.h"
#include "p2p_network.h"
#include <atomic>
#include <condition_variable>

class BlockchainNode {
    public:
        BlockchainNode(int port);
        void start();
        void stop();

        void connectToPeer(const std::string& ipAddress, int port);
        void handleNewBlock(const Block& block);
        void addTransactionToBlock(const Transaction& tx);
        bool hasPendingTransaction() const;
        void notifyBlockMined(const Block& block);
    private:
        Block pendingBlock;
        Blockchain blockchain;
        P2PNetwork p2pNetwork;
        std::atomic<bool> shouldStop{false};
        std::condition_variable stopCondition;
        std::condition_variable mineCondition;
        std::mutex stopMutex;
        std::mutex mineMutex;
        bool miningEnabled;
        bool shouldMine{false};
        void mine();
};

#endif