#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <limits>

#include "card.h"
#include "deck.h"

int Card::next_id = 1;

int main() {
    Deck myDeck;
    int choice;

    std::cout << "\n";
    while(true) {
        std::cout << "1. add card\n";
        std::cout << "2. review cards\n";
        std::cout << "3. skip day (test)\n";
        std::cout << "4. browse deck\n";
        std::cout << "5. delete card\n";
        std::cout << "6. exit\n";
        std::cout << "\n";

        std::cin >> choice;
        std::cout << "\n";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            std::string front, back;
            std::cout << "enter front:\n";
            getline(std::cin, front);
            std::cout << "enter back:\n";
            getline(std::cin, back);
            std::cout << "\n";

            myDeck.add_card(std::move(front), std::move(back));
        }
        else if (choice == 2) {
            myDeck.review_cards();
        }
        else if (choice == 3) {
            myDeck.skip_day();
        }
        else if (choice == 4) {
            myDeck.browse();
        }
        else if (choice == 5) {
            int target_id;
            std::cout << "enter id of card to be deleted:\n";
            std::cout << "(can be found in card browser)\n";
            std::cin >> target_id;
            myDeck.delete_card(target_id);
        }
        else if (choice == 6) {
            break;
        }
        else {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "invalid!\n";
        }
    }
}