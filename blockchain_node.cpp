// blockchain_node.cpp

#include "blockchain_node.h"
#include "p2p_network.h"
#include <iostream>
#include <thread>

void BlockchainNode::connectToInitialPeers() {
    connectToPeer("127.0.0.1", 8081);
    connectToPeer("127.0.0.1", 8082);
}

void BlockchainNode::connectToPeer(const std::string& ipAddress, int port) {
    p2pNetwork.connectToPeer(ipAddress, port);
    // ...
}

BlockchainNode::BlockchainNode(int port) {
    p2pNetwork.startServer();
}

void BlockchainNode::start() {
    // Connect to initial peers (for simplicity, we connect to two peers here)
    // connectToInitialPeers();

    while (true) {
        // Simulate mining a new block
        Block newBlock;
        newBlock.index = blockchain.chain.size() + 1;
        newBlock.previousHash = blockchain.chain.empty() ? "0000000000000000" : blockchain.chain.back().hash;
        newBlock.timestamp = std::time(0);
        newBlock.nonce = 0;

        // Add some transactions (for simplicity, we add a static one here)
        Transaction tx;
        tx.sender = "Alice";
        tx.receiver = "Bob";
        tx.amount = 1.5;
        newBlock.transactions.push_back(tx);

        // Mine the block with a difficulty of 4 (for simplicity)
        int difficulty = 4;
        std::string minedHash = mineBlock(newBlock, difficulty);
        newBlock.hash = minedHash;

        // Add the block to the local blockchain
        blockchain.addBlock(newBlock);

        // Broadcast the new block to all connected peers
        p2pNetwork.broadcast(serializeBlock(newBlock));

        std::cout << "Mined new block with hash: " << newBlock.hash << std::endl;

        // Add a delay to simulate mining time between blocks
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

void BlockchainNode::handleNewBlock(const Block& block) {
    // Check if the received block is valid and can be added to the local blockchain
    if (blockchain.isValidNewBlock(block)) {
        blockchain.addBlock(block);
        std::cout << "Added new block with hash: " << block.hash << std::endl;
    } else {
        std::cout << "Received invalid block, discarding..." << std::endl;
    }
}