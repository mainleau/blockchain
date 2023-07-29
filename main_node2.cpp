// main_node2.cpp

#include "blockchain_node.h"
#include "transaction_window.h"
#include <gtkmm/application.h>
#include <thread>

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv);

    TransactionWindow transactionWindow;

    BlockchainNode node2(9001);
    node2.connectToPeer("127.0.0.1", 9000);
    
    std::thread nodeThread([&]() {
        node2.start();
    });

    // Set the blockchain node and the condition variable for the transaction window
    transactionWindow.setBlockchainNode(&node2);

    int result = app->run(transactionWindow);

    node2.stop();
    nodeThread.join();

    return result;
}