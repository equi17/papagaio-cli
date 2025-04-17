#include "cardmanager.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <ctime>

// public member functions
CardManager::CardManager(const std::string& dbPath): db_(dbPath) {
    decks_ = db_.loadAllDecks();
}

void CardManager::add_deck() {
    std::string name = get_string_input("enter deck name:\n");

    auto it = std::find_if(decks_.begin(), decks_.end(), [&name](const Deck& deck){ 
        return deck.name == name; 
    });

    if (it != decks_.end()) {
        std::cout << "there's already a deck with this name!\n";
        return;
    }

    Deck newDeck(name);
    db_.saveDeck(newDeck);
    decks_.push_back(newDeck);
}

void CardManager::delete_deck() {
    display_decks();
    if(decks_.empty()) {
        std::cout << "no decks to delete!\n";
        return;
    }

    int target_id = get_int_input("enter id of deck to delete:\n");
    
    try {
        db_.deleteDeck(target_id);

        auto it = std::find_if(decks_.begin(), decks_.end(), 
            [target_id](const Deck& d){ return d.get_id() == target_id; }
        );

        if (it != decks_.end()) {
            decks_.erase(it);
            std::cout << "deck deleted\n";
            return;
        }
        std::cout << "deck deleted from DB but not found in memory!\n";
    } catch (const std::exception& e) {
        std::cerr << "error deleting deck: " << e.what() << "\n";
    }
}

void CardManager::add_card(Deck& deck) {
    std::string front = get_string_input("enter front:\n");
    std::string back = get_string_input("enter back:\n");

    if (deck.card_exists(front)) {
        std::cout << "card with this front already exists in this deck!\n";
        return;
    }

    try {
        Card new_card(front, back, deck.get_id());
        
        db_.saveCard(new_card, deck.get_id());
        
        deck.add_card(new_card);
        
        std::cout << "card added successfully (id: " << new_card.get_id() << ")\n";
    } catch (const std::exception& e) {
        std::cerr << "failed to add card: " << e.what() << "\n";
    }
}

void CardManager::review_cards(Deck& deck) {
    deck.review_cards([this, &deck](Card& card){
        std::cout << "front: " << card.front << "\n";
        std::string answer = get_string_input("your answer: ");
        std::cout << "back: " << card.back << "\n";
 
        std::string correct = get_string_input("did you get it right? (y/n): ");
        time_t now = std::time(nullptr) + deck.get_days_skipped() * 86400;
 
        card.update((correct == "y" || correct == "Y"), deck.get_days_skipped());
        db_.updateCard(card);
    });
}

void CardManager::skip_day() {
    for(auto& d: decks_) {
        d.skip_day();
        db_.updateDeck(d);
    }
}

void CardManager::browse_cards(Deck& deck) {
    deck.browse();
}

void CardManager::delete_card(Deck& deck) {
    int target_id = get_int_input("enter id of card to delete:\n");
    
    try {
        db_.deleteCard(target_id);
        
        if (!deck.delete_card(target_id)) {
            std::cout << "card deleted from DB but not found in memory!\n";
        } else {
            std::cout << "card deleted\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "error deleting card: " << e.what() << "\n";
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
              << "2. delete deck\n"
              << "3. select deck\n"
              << "4. skip day (for testing)\n"
              << "5. exit\n"
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
    Deck& current_deck = find_deck_by_id(deck_id)->get();
    while (true) {
        std::cout << "\n"
                  << "you are currently in deck \"" << current_deck.name << "\"\n"
                  << "1. add card\n"
                  << "2. review cards\n"
                  << "3. browse cards\n"
                  << "4. delete card\n"
                  << "5. back to deck selection\n";
        
        int choice = get_int_input("enter your choice:\n");
        switch (choice) {
            case 1:
                add_card(current_deck);
                break;
            case 2:
                review_cards(current_deck);
                break;
            case 3:
                browse_cards(current_deck);
                break;
            case 4:
                delete_card(current_deck);
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