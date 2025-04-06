#include "cardmanager.h"
#include <iostream>
#include <limits>

// public member functions
CardManager::CardManager(Deck& deck): deck_(deck) {}

void CardManager::add_card() {
    std::string front = get_string_input("enter front:\n");
    std::string back = get_string_input("enter front:\n");
    deck_.add_card(front, back);
}

void CardManager::review_cards() {
    deck_.review_cards([this](Card& card){
         std::cout << "front: " << card.front << "\n";
         std::string answer = get_string_input("your answer: ");
         std::cout << "back: " << card.back << "\n";
 
         std::string correct = get_string_input("did you get it right? (y/n): ");
         time_t now = std::time(nullptr) + deck_.get_days_skipped() * 86400;
 
         if (correct == "y" || correct == "Y") {
             card.update(true, deck_.get_days_skipped());
         } else {
             card.update(false, deck_.get_days_skipped());
         }
    });
}

void CardManager::skip_day() {
    deck_.skip_day();
}

void CardManager::browse_cards() {
    deck_.browse();
}

void CardManager::delete_card() {
    int target_id = get_int_input("enter id of card to delete:\n");
    bool error = deck_.delete_card(target_id);
    if(error) {
        std::cout << "no card with such id!\n";
    } else {
        std::cout << "card deleted\n";
    }
}

int CardManager::get_int_input(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if(std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "error! please enter a number\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
    }
}

std::string CardManager::get_string_input(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

void CardManager::display_menu() const {
    std::cout << "\n"
              << "1. add card\n"
              << "2. review cards\n"
              << "3. skip day (test)\n"
              << "4. browse deck\n"
              << "5. delete card\n"
              << "6. exit\n"
              << "\n";
}