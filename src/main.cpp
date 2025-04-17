#include <iostream>
#include <vector>
#include "cardmanager.h"
#include "databasemanager.h"

int Card::next_id = 1;
int Deck::next_id = 1;

int main() {
    CardManager manager("data.db");

    while (true) {
        manager.display_main_menu();
        int choice = manager.get_int_input("enter your choice: ");

        switch (choice) {
            case 1: {
                manager.add_deck();
                break;
            }
            case 2: {
                int deck_id = manager.choose_deck();
                if (deck_id != -1) {
                    manager.display_deck_menu(deck_id);
                } else {
                    std::cout << "no deck with such id!\n";
                }
                break;
            }
            case 3:
                manager.skip_day();
                break;
            case 4:
                return 0;
            default: 
                std::cout << "invalid choice!\n";
        }
    }
}