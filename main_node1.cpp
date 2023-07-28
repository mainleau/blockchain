// main_node1.cpp
#include "blockchain_node.h"
#include "transaction_window.h"
#include <gtkmm/application.h>
#include <thread> // Ajoutez cette ligne pour inclure la bibliothèque std::thread

int main(int argc, char* argv[]) {
    // Créez une instance de Gtk::Application
    auto app = Gtk::Application::create(argc, argv);

    // Instanciez la fenêtre de transaction
    TransactionWindow transactionWindow;
    // Connectez-vous au réseau P2P initial
    
    // Instanciez le nœud blockchain dans un thread séparé
    std::thread nodeThread([&]() {
        BlockchainNode node1(8080);
        node1.start();
        transactionWindow.setBlockchainNode(&node1);
    });

    // Exécutez l'application GTK et affichez la fenêtre de transaction
    int result = app->run(transactionWindow);

    // Attendez que le thread du nœud se termine
    nodeThread.join();

    return result;
}
