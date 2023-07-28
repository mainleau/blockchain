#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <string>
#include <vector>
#include <ctime>

// Structure pour représenter une transaction
struct Transaction {
    std::string sender;
    std::string receiver;
    double amount;
};

// Structure pour représenter un bloc
struct Block {
    int index;
    std::string previousHash;
    std::time_t timestamp;
    std::vector<Transaction> transactions;
    int nonce;
    std::string hash;
};

class Blockchain {
    public:
        Blockchain();

        void addBlock(const Block& block);
        bool isValidNewBlock(const Block& newBlock);
        Block getLatestBlock() const;
        std::vector<Block> chain;
    private:
    };


std::string calculateHash(const std::string& input);
std::string serializeBlock(const Block& block);
Block deserializeBlock(const std::string& data);
std::string mineBlock(Block& block, int difficulty);
void broadcastBlock(const Block& block, std::vector<Block>& blockchain);

#endif // BLOCKCHAIN_H
