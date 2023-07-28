// transaction_window.h

#ifndef TRANSACTION_WINDOW_H
#define TRANSACTION_WINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/entry.h>
#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include <string>
#include "blockchain.h"

class BlockchainNode; // Déclaration anticipée de la classe BlockchainNode

class TransactionWindow : public Gtk::Window {
public:
    TransactionWindow(); // Constructeur prenant un pointeur vers BlockchainNode
    virtual ~TransactionWindow();
    void setBlockchainNode(BlockchainNode* node);
private:
    // Pointeur vers l'instance BlockchainNode
    BlockchainNode* blockchainNode;

    // Widgets de l'interface graphique
    Gtk::Box mainLayout;
    Gtk::Entry entry_sender;
    Gtk::Entry entry_receiver;
    Gtk::Entry entry_amount;
    Gtk::Button button_send;

    // Fonction de rappel du bouton "Send"
    void on_button_send_clicked();
};

#endif // TRANSACTION_WINDOW_H
