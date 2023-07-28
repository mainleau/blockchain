// main_node2.cpp

#include "blockchain_node.h"

int main() {
    BlockchainNode node2(8081);
    node2.connectToPeer("127.0.0.1", 8080); // Connect to node1
    node2.start();

    return 0;
}
