#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <string>
#include <vector>
#include <ctime>

struct Transaction {
    std::string sender;
    std::string receiver;
    double amount;
};

struct Block {
    int index;
    int nonce;
    std::time_t timestamp;
    std::vector<Transaction> transactions;
    std::string previousHash;
    std::string hash;
};

class Blockchain {
    public:
        Blockchain();

        void addBlock(const Block& block);
        bool isValidNewBlock(const Block& newBlock);
        Block getLatestBlock() const;
        bool isEmpty();
        int getSize();
    private:
        std::vector<Block> chain;
    };


std::string calculateHash(const std::string& input);
std::string serializeBlock(const Block& block);
Block deserializeBlock(const std::string& data);
std::string mineBlock(Block& block, int difficulty);
void broadcastBlock(const Block& block, std::vector<Block>& blockchain);

#endif
