// blockchain_node.cpp

#include "blockchain_node.h"
#include "blockchain.h"
#include "p2p_network.h"
#include <iostream>
#include <thread>
#include <sstream>

void BlockchainNode::connectToInitialPeers() {
    connectToPeer("127.0.0.1", 8081);
    connectToPeer("127.0.0.1", 8082);
}

bool BlockchainNode::hasPendingTransaction() const {
    return pendingTransaction ? true : false;
}

void BlockchainNode::connectToPeer(const std::string& ipAddress, int port) {
    p2pNetwork.connectToPeer(ipAddress, port);
}

BlockchainNode::BlockchainNode(int port) {
    p2pNetwork.startServer();
}

void BlockchainNode::sendTransactionToNetwork(const Transaction& transaction) {
    // Serialize the transaction
    std::string serializedTx = serializeTransaction(transaction);

    // Broadcast the serialized transaction to all connected peers
    p2pNetwork.broadcast(serializedTx);
}

std::string BlockchainNode::serializeTransaction(const Transaction& transaction) {
    std::stringstream ss;
    ss << transaction.sender << ";"
       << transaction.receiver << ";"
       << transaction.amount;
    return ss.str();
}

void BlockchainNode::start() {
    while (true) {

        if(hasPendingTransaction()) {
            Block newBlock;
            newBlock.index = blockchain.getSize() + 1;
            newBlock.previousHash = blockchain.isEmpty() ? "0000000000000000" : blockchain.getLatestBlock().hash;
            newBlock.timestamp = std::time(0);
            newBlock.nonce = 0;

            Transaction tx;
            tx.sender = "Alice";
            tx.receiver = "Bob";
            tx.amount = 1.5;
            newBlock.transactions.push_back(tx);

            int difficulty = 4;
            std::string minedHash = mineBlock(newBlock, difficulty);
            newBlock.hash = minedHash;

            blockchain.addBlock(newBlock);

            // Broadcast the new block to all connected peers
            p2pNetwork.broadcast(serializeBlock(newBlock));

            std::cout << "Mined new block with hash: " << newBlock.hash << std::endl;
        }

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