#include <iostream>
#include <vector>
#include <ctime>
#include <chrono>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include "blockchain.h"

int main() {
    std::vector<Block> blockchain; // Blockchain du nœud 1

    // Création du socket pour communiquer avec le nœud 2
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Erreur lors de la création du socket" << std::endl;
        return 1;
    }

    // Connexion au nœud 2 (localhost, port 8080)
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Erreur lors de la connexion au nœud 2" << std::endl;
        close(sockfd);
        return 1;
    }

    std::cout << "Connexion établie avec le nœud 2" << std::endl;

    // Boucle infinie pour continuer le minage et la communication avec le nœud 2
    while (true) {
        // Création d'un bloc de transaction pour le nœud 1
        Block node1Block;
        node1Block.index = blockchain.size() + 1;
        node1Block.previousHash = blockchain.empty() ? "0000000000000000" : blockchain.back().hash;
        node1Block.timestamp = std::time(0);
        node1Block.nonce = 0;

        // Ajout de transactions (pour simplifier, nous en ajoutons une statique ici)
        Transaction tx;
        tx.sender = "Alice";
        tx.receiver = "Bob";
        tx.amount = 1.5;
        node1Block.transactions.push_back(tx);

        // Minage du bloc avec une difficulté de 4 (pour la simplicité)
        int difficulty = 4;
        std::string minedHash = mineBlock(node1Block, difficulty);

        // Diffusion du bloc nouvellement miné à tous les nœuds du réseau
        broadcastBlock(node1Block, blockchain);

        // Envoi du bloc au nœud 2 via le socket
        std::string serializedBlock = serializeBlock(node1Block);
        send(sockfd, serializedBlock.c_str(), serializedBlock.size(), 0);

        std::cout << "Bloc envoyé au nœud 2 : " << serializedBlock << std::endl;

        // Ajout d'un délai pour simuler le temps de minage entre les blocs
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    close(sockfd); // Fermeture du socket avant de quitter

    return 0;
}
