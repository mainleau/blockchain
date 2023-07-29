// transaction_window.cpp

#include "transaction_window.h"
#include "blockchain_node.h"
#include <gtkmm/box.h>

TransactionWindow::TransactionWindow() {
    set_default_size(300, 200);
    set_title("Send Transaction");

    mainLayout.pack_start(entry_sender, Gtk::PACK_SHRINK);
    mainLayout.pack_start(entry_receiver, Gtk::PACK_SHRINK);
    mainLayout.pack_start(entry_amount, Gtk::PACK_SHRINK);
    mainLayout.pack_start(button_send, Gtk::PACK_SHRINK);

    entry_sender.set_text("Alice");
    entry_receiver.set_text("Bob");
    entry_amount.set_text("1.5");

    button_send.set_label("Send");
    add(mainLayout);

    button_send.signal_clicked().connect([&]() {
        on_button_send_clicked();
    });

    show_all_children();
}

void TransactionWindow::setBlockchainNode(BlockchainNode* node) {
    blockchainNode = node;
}

TransactionWindow::~TransactionWindow() {}

void TransactionWindow::on_button_send_clicked() {
    std::string sender = entry_sender.get_text();
    std::string receiver = entry_receiver.get_text();
    double amount = std::stod(entry_amount.get_text());


    // Create the transaction
    Transaction tx;
    tx.sender = sender;
    tx.receiver = receiver;
    tx.amount = amount;

    // Send the transaction to the network using blockchainNode
    if (blockchainNode) {
        blockchainNode->addTransactionToBlock(tx);
    }
}
