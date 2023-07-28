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
    // Créer le bloc génésis (premier bloc de la blockchain)
    Block genesisBlock;
    genesisBlock.index = 1;
    genesisBlock.previousHash = "0000000000000000";
    genesisBlock.timestamp = std::time(0);
    genesisBlock.nonce = 0;
    genesisBlock.hash = calculateHash(serializeBlock(genesisBlock));
    chain.push_back(genesisBlock);
}

void Blockchain::addBlock(const Block& block) {
    chain.push_back(block);
}

bool Blockchain::isValidNewBlock(const Block& newBlock) {
    // Vérifier si le nouvel index est correct
    if (newBlock.index != chain.size() + 1) {
        return false;
    }

    // Vérifier si le nouvel hash est valide en fonction des preuves de travail (difficulté)
    int difficulty = 4; // Valeur de la difficulté pour le minage (pour la simplicité)
    std::string target(difficulty, '0');
    std::string hash = calculateHash(serializeBlock(newBlock));
    if (hash.substr(0, difficulty) != target) {
        return false;
    }

    // Vérifier si le hash précédent correspond au dernier bloc de la chaîne
    const Block& lastBlock = chain.back();
    if (newBlock.previousHash != lastBlock.hash) {
        return false;
    }

    return true;
}

Block Blockchain::getLatestBlock() const {
    return chain.back();
}

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

// Fonction pour sérialiser un bloc en chaîne de caractères
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

// Fonction pour désérialiser une chaîne de caractères en un bloc
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
        // std::cout << "Hash: " << ss.str() << std::endl;
        hash = calculateHash(ss.str());
    } while (hash.substr(0, difficulty) != target);
    return hash;
}

// Fonction pour diffuser un bloc nouvellement miné à tous les nœuds du réseau
void broadcastBlock(const Block& block, std::vector<Block>& blockchain) {
    // Dans un réseau réel, cette fonction enverrait le bloc à tous les autres nœuds du réseau
    // Pour cet exemple, nous allons simplement ajouter le bloc à tous les nœuds de notre réseau local
    blockchain.push_back(block);
}