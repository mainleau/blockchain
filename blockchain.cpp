#include "blockchain.h"
#include <string>
#include <vector>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <openssl/evp.h>
#include <thread>
#include <chrono>
#include <iostream>

Blockchain::Blockchain() {
    Block genesisBlock;
    genesisBlock.index = 1;
    genesisBlock.nonce = 0;
    genesisBlock.timestamp = std::time(0);
    genesisBlock.previousHash = "0000000000000000000000000000000000000000000000000000000000000000";
    genesisBlock.hash = calculateHash(serializeBlock(genesisBlock));
    chain.push_back(genesisBlock);
}

void Blockchain::addBlock(const Block& block) {
    chain.push_back(block);
}

bool Blockchain::isEmpty() {
    return chain.empty();
}

int Blockchain::getSize() {
    return chain.size();
}

bool Blockchain::isValidNewBlock(const Block& newBlock) {
    if (newBlock.index != chain.size() + 1) {
        return false;
    }

    int difficulty = 4;
    std::string target(difficulty, '0');
    std::string hash = calculateHash(serializeBlock(newBlock));
    if (hash.substr(0, difficulty) != target) {
        return false;
    }

    const Block& lastBlock = chain.back();
    if (newBlock.previousHash != lastBlock.hash) {
        return false;
    }

    return true;
}

Block Blockchain::getLatestBlock() const {
    return chain.back();
}

std::string calculateHash(const std::string& input) {
    EVP_MD_CTX* mdctx;
    const EVP_MD* md;
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    md = EVP_sha256();
    mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, input.c_str(), input.length());
    EVP_DigestFinal_ex(mdctx, hash, &hash_len);
    EVP_MD_CTX_free(mdctx);

    std::stringstream ss;
    for (unsigned int i = 0; i < hash_len; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

std::string serializeBlock(const Block& block) {
    std::stringstream ss;
    ss << block.index << ';'
       << block.previousHash << ';'
       << block.timestamp << ';'
       << block.nonce << ';'
       << block.hash;

    for (const Transaction& tx : block.transactions) {
        ss << ';'
           << tx.sender << ';'
           << tx.receiver << ';'
           << tx.amount;
    }

    return ss.str();
}

Block deserializeBlock(const std::string& serializedBlock) {
    std::stringstream ss(serializedBlock);
    Block block;
    std::string field;

    std::getline(ss, field, ';');
    block.index = std::stoi(field);

    std::getline(ss, block.previousHash, ';');

    std::getline(ss, field, ';');
    block.timestamp = std::stoi(field);

    std::getline(ss, field, ';');
    block.nonce = std::stoi(field);

    std::getline(ss, block.hash, ';');

    while (std::getline(ss, field, ';')) {
        Transaction tx;
        tx.sender = field;

        std::getline(ss, tx.receiver, ';');

        std::getline(ss, field, ';');
        tx.amount = std::stod(field);

        block.transactions.push_back(tx);
    }

    return block;
}

std::string mineBlock(Block& block, int difficulty) {
    std::string target(difficulty, '0');
    std::string hash;
    do {
        block.nonce++;
        std::stringstream ss;
        ss << block.index << block.previousHash << block.timestamp << block.nonce;
        for (const auto& tx : block.transactions) {
            ss << tx.sender << tx.receiver << tx.amount;
        }
        hash = calculateHash(ss.str());
    } while (hash.substr(0, difficulty) != target);
    return hash;
}

void broadcastBlock(const Block& block, std::vector<Block>& blockchain) {
    blockchain.push_back(block);
}