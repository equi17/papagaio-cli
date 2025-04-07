#include <iostream>
#include <vector>
#include "cardmanager.h"

int Card::next_id = 1;
int Deck::next_id = 1;

int main() {
    std::vector<Deck> myDecks;
    CardManager manager(myDecks);

    while (true) {
        manager.display_menu();
        int choice = manager.get_int_input("enter your choice: ");

        switch (choice) {
            case 0: {
                manager.add_deck();
                break;
            }
            case 1: {
                int id = manager.choose_deck(); 
                manager.add_card(id); 
                break;
            }
            case 2: {
                int id = manager.choose_deck(); 
                manager.review_cards(id); 
                break;
            }
            case 3: {
                manager.skip_day(); 
                break;
            }
            case 4: {
                int id = manager.choose_deck(); 
                manager.browse_cards(id); 
                break;
            }
            case 5: {
                int id = manager.choose_deck(); 
                manager.delete_card(id); 
                break;
            }
            case 6: 
                return 0;
            default: 
                std::cout << "invalid choice!\n";
        }
    }
}