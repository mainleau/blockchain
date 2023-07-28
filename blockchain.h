#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <string>
#include <vector>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <openssl/evp.h>
#include <thread>
#include <chrono>


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

// Fonction pour calculer le hash SHA-256 d'une chaîne
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

// Fonction pour générer le hash d'un bloc en effectuant le minage
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
        std::cout << "Hash: " << ss.str() << std::endl;
        hash = calculateHash(ss.str());
    } while (hash.substr(0, difficulty) != target);
    return hash;
}

// Fonction pour ajouter un bloc au réseau en tant que nœud
void addBlockToNode(Block& block, std::vector<Block>& blockchain) {
    blockchain.push_back(block);
}

// Fonction pour diffuser un bloc nouvellement miné à tous les nœuds du réseau
void broadcastBlock(const Block& block, std::vector<Block>& blockchain) {
    // Dans un réseau réel, cette fonction enverrait le bloc à tous les autres nœuds du réseau
    // Pour cet exemple, nous allons simplement ajouter le bloc à tous les nœuds de notre réseau local
    blockchain.push_back(block);
}

// Fonction pour sérialiser un bloc en chaîne de caractères
std::string serializeBlock(const Block& block) {
    std::stringstream ss;
    ss << block.index << block.previousHash << block.timestamp << block.nonce << block.hash;

    for (const auto& tx : block.transactions) {
        ss << tx.sender << tx.receiver << tx.amount;
    }

    return ss.str();
}

// Fonction pour désérialiser une chaîne de caractères en un bloc
Block deserializeBlock(const std::string& serializedBlock) {
    std::stringstream ss(serializedBlock);
    Block block;
    ss >> block.index >> block.previousHash >> block.timestamp >> block.nonce >> block.hash;

    while (!ss.eof()) {
        Transaction tx;
        ss >> tx.sender >> tx.receiver >> tx.amount;
        block.transactions.push_back(tx);
    }

    return block;
}

#endif // BLOCKCHAIN_H