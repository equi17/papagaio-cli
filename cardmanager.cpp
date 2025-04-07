#include "cardmanager.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <ctime>

// public member functions
CardManager::CardManager(std::vector<Deck> decks): decks_(std::move(decks)) {}

void CardManager::add_deck() {
    std::string name = get_string_input("enter deck name:\n");

    auto it = std::find_if(decks_.begin(), decks_.end(), [&name](const Deck& deck){ 
        return deck.name == name; 
    });

    if (it != decks_.end()) {
        std::cout << "there's already a deck with this name!\n";
        return;
    }

    decks_.emplace_back(name);
}

void CardManager::add_card(int id) {
    if (decks_.empty()) {
        std::cout << "no decks created!\n";
        return;
    }

    std::string front = get_string_input("enter front:\n");
    std::string back = get_string_input("enter back:\n");

    auto deck = find_deck_by_id(id);
    if(!deck) {
        std::cout << "there's no deck with such id!\n";
        return;
    }

    bool error = deck->get().add_card(front, back);
    if (error)
        std::cout << "there's already a card with this front side!\n";
}

void CardManager::review_cards(int id) {
    auto deck = find_deck_by_id(id);
    if(!deck) {
        std::cout << "there's no deck with such id!\n";
        return;
    }

    deck->get().review_cards([this, &deck](Card& card){
        std::cout << "front: " << card.front << "\n";
        std::string answer = get_string_input("your answer: ");
        std::cout << "back: " << card.back << "\n";
 
        std::string correct = get_string_input("did you get it right? (y/n): ");
        time_t now = std::time(nullptr) + deck->get().get_days_skipped() * 86400;
 
        card.update((correct == "y" || correct == "Y"), deck->get().get_days_skipped());
    });
}

void CardManager::skip_day() {
    for(auto& d: decks_)
        d.skip_day();
}

void CardManager::browse_cards(int id) {
    auto deck = find_deck_by_id(id);
    if(!deck) {
        std::cout << "there's no deck with such id!\n";
        return;
    }
    deck->get().browse();
}

void CardManager::delete_card(int id) {
    auto deck = find_deck_by_id(id);
    if(!deck) {
        std::cout << "there's no deck with such id!\n";
        return;
    }

    int target_id = get_int_input("enter id of card to delete:\n");
    bool error = deck->get().delete_card(target_id);

    if(error) {
        std::cout << "no card with such id!\n";
    } else {
        std::cout << "card deleted\n";
    }
}

int CardManager::get_int_input(const std::string& prompt) const {
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

std::string CardManager::get_string_input(const std::string& prompt) const {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

void CardManager::display_main_menu() const {
    std::cout << "\n"
              << "1. add deck\n"
              << "2. select deck\n"
              << "3. skip day (for testing)\n"
              << "4. exit\n"
              << "\n";
}

int CardManager::choose_deck() {
    display_decks();
    int deck_id =  get_int_input("choose deck id:\n");
    auto deck = find_deck_by_id(deck_id);
    if(!deck) return -1;
    return deck_id;
}

void CardManager::display_deck_menu(int deck_id) {
    while (true) {
        std::cout << "\n"
                  << "1. add card\n"
                  << "2. review cards\n"
                  << "3. browse cards\n"
                  << "4. delete card\n"
                  << "5. back to deck selection\n";
        
        int choice = get_int_input("enter your choice:\n");
        switch (choice) {
            case 1:
                add_card(deck_id);
                break;
            case 2:
                review_cards(deck_id);
                break;
            case 3:
                browse_cards(deck_id);
                break;
            case 4:
                delete_card(deck_id);
                break;
            case 5:
                return;
            default:
                std::cout << "invalid choice!\n";
        }
    }
}

// private member functions
std::optional<std::reference_wrapper<Deck>> CardManager::find_deck_by_id(int id) {
    auto it = std::find_if(decks_.begin(), decks_.end(), 
        [id](Deck& d){ return d.get_id() == id; });
    
    if (it == decks_.end()) return std::nullopt;
    return *it;
}

void CardManager::display_decks() const {
    for (const auto deck: decks_) {
        std::cout << deck.name << " id: " << deck.get_id() << '\n';
    }
}