// main_node3.cpp

#include "blockchain_node.h"

int main() {
    BlockchainNode node3(8082);
    node3.connectToPeer("127.0.0.1", 8080); // Connect to node1
    node3.connectToPeer("127.0.0.1", 8081); // Connect to node2
    node3.start();

    return 0;
}
