// blockchain_node.cpp

#include "blockchain_node.h"
#include "blockchain.h"
#include "p2p_network.h"
#include <iostream>
#include <thread>
#include <sstream>

BlockchainNode::BlockchainNode(int port) {
    std::thread serverThread(&P2PNetwork::startServer, &p2pNetwork, port);
    serverThread.detach();
    p2pNetwork.startServer(port);
    pendingBlock = blockchain.getLatestBlock();
}

void BlockchainNode::connectToPeer(const std::string& ipAddress, int port) {
    p2pNetwork.connectToPeer(ipAddress, port);
}

void BlockchainNode::addTransactionToBlock(const Transaction& tx) {
    pendingBlock.transactions.push_back(tx);
}

bool BlockchainNode::hasPendingTransaction() const {
    return !pendingBlock.transactions.empty();
}

void BlockchainNode::handleNewBlock(const Block& block) {
    if (blockchain.isValidNewBlock(block)) {
        blockchain.addBlock(block);
        std::cout << "Added new block with hash: " << block.hash << std::endl;
    } else {
        std::cout << "Received invalid block, discarding..." << std::endl;
    }
}

void BlockchainNode::start() {
    std::thread miningThread(&BlockchainNode::mine, this);

    while (!shouldStop) {
        std::unique_lock<std::mutex> lock(mineMutex);
        mineCondition.wait(lock, [&]() { return shouldMine || shouldStop; });

        if (shouldStop) {
            break;
        }

        if (hasPendingTransaction()) {
            Block newBlock = pendingBlock;
            int difficulty = 4;
            std::string minedHash = mineBlock(newBlock, difficulty);
            newBlock.hash = minedHash;

            blockchain.addBlock(newBlock);
            p2pNetwork.broadcast(serializeBlock(newBlock));

            std::cout << "Mined new block with hash: " << newBlock.hash << std::endl;

            // Clear the pending transactions after mining
            pendingBlock.index = newBlock.index + 1;
            pendingBlock.nonce = 0;
            pendingBlock.timestamp = std::time(0);
            pendingBlock.previousHash = newBlock.hash;
            pendingBlock.transactions.clear();
        }

        shouldMine = false; // Réinitialiser l'indicateur de minage
    }

    miningThread.join(); // Attendre que le thread de minage se termine
}

void BlockchainNode::notifyBlockMined(const Block& block) {
    p2pNetwork.broadcast(serializeBlock(block));
}

void BlockchainNode::mine() {
    while (!shouldStop) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        {
            std::unique_lock<std::mutex> lock(mineMutex);
            shouldMine = true; // Signaler au thread principal de démarrer le minage
        }
        mineCondition.notify_one();
    }
}

void BlockchainNode::stop() {
    {
        std::lock_guard<std::mutex> lock(stopMutex);
        shouldStop = true;
    }
    mineCondition.notify_one(); // Notifier le thread de minage pour qu'il puisse s'arrêter
}