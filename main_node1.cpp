// main_node1.cpp
#include "blockchain_node.h"
#include "transaction_window.h"
#include <gtkmm/application.h>
#include <thread>

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv);

    TransactionWindow transactionWindow;

    BlockchainNode node1(8080);
    node1.connectToPeer("127.0.0.1", 8081);
    
    std::thread nodeThread([&]() {
        node1.start();
    });

    // Set the blockchain node and the condition variable for the transaction window
    transactionWindow.setBlockchainNode(&node1);

    int result = app->run(transactionWindow);

    node1.stop();
    nodeThread.join();

    return result;
}