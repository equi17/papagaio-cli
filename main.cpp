#include <iostream>
#include "cardmanager.h"

int Card::next_id = 1;

int main() {
    Deck myDeck;
    CardManager manager(myDeck);

    while (true) {
        manager.display_menu();
        int choice = manager.get_int_input("enter your choice: ");

        switch (choice) {
            case 1: manager.add_card(); break;
            case 2: manager.review_cards(); break;
            case 3: manager.skip_day(); break;
            case 4: manager.browse_cards(); break;
            case 5: manager.delete_card(); break;
            case 6: return 0;
            default: std::cout << "invalid choice!\n";
        }
    }
}