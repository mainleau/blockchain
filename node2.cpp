#include <iostream>
#include <vector>
#include <ctime>
#include <chrono>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <mutex>
#include "blockchain.h"

std::mutex blockchainMutex; // Mutex pour protéger l'accès à la blockchain

// Fonction pour gérer une connexion de nœud 1
void handleConnection(int clientSockfd, std::vector<Block>& blockchain) {
    std::cout << "Connexion établie avec le nœud 1" << std::endl;

    while (true) {
        // Réception du bloc depuis le nœud 1 via le socket
        char buffer[1024] = {0};
        int bytesReceived = recv(clientSockfd, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            // Erreur ou connexion fermée par le nœud 1, fermeture du socket client
            close(clientSockfd);
            std::cerr << "Erreur lors de la réception du bloc ou connexion fermée par le nœud 1" << std::endl;
            return;
        }

        std::cout << "Bloc reçu du nœud 1 : " << buffer << std::endl;

        // Désérialisation du bloc reçu
        std::string serializedBlock(buffer, bytesReceived);
        Block receivedBlock = deserializeBlock(serializedBlock);

        // Ajout du bloc reçu à la blockchain du nœud 2 (sous verrou pour éviter les conflits)
        std::lock_guard<std::mutex> lock(blockchainMutex);
        blockchain.push_back(receivedBlock);

        // Affichage de la blockchain dans le nœud 2
        std::cout << "Blockchain du noeud 2 : " << std::endl;
        for (const auto& block : blockchain) {
            std::cout << "Hash : " << block.hash << std::endl;
        }
    }
}

int main() {
    std::vector<Block> blockchain; // Blockchain du nœud 2

    // Création du socket pour accepter les connexions des autres nœuds
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Erreur lors de la création du socket" << std::endl;
        return 1;
    }

    // Configuration du socket pour écouter les connexions entrantes (localhost, port 8080)
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Erreur lors de la liaison du socket" << std::endl;
        return 1;
    }

    if (listen(sockfd, 5) < 0) {
        std::cerr << "Erreur lors de l'écoute du socket" << std::endl;
        return 1;
    }

    std::cout << "Nœud 2 en attente de connexions..." << std::endl;

    // Boucle pour accepter les connexions entrantes et gérer chaque connexion avec un thread dédié
    while (true) {
        int clientSockfd = accept(sockfd, NULL, NULL);
        if (clientSockfd < 0) {
            std::cerr << "Erreur lors de l'acceptation d'une connexion" << std::endl;
            continue;
        }

        std::thread connectionThread(handleConnection, clientSockfd, std::ref(blockchain));
        connectionThread.detach(); // Détacher le thread pour le laisser s'exécuter indépendamment
    }

    close(sockfd); // Fermeture du socket avant de quitter

    return 0;
}
