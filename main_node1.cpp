// main_node1.cpp

#include "blockchain_node.h"
#include "transaction_window.h"
#include <gtkmm/application.h>
#include <thread>

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv);

    TransactionWindow transactionWindow;
    
    std::thread nodeThread([&]() {
        BlockchainNode node1(8080);
        node1.start();
        transactionWindow.setBlockchainNode(&node1);
    });

    int result = app->run(transactionWindow);

    nodeThread.join();

    return result;
}
