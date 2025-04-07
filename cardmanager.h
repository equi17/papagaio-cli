#pragma once
#include <string>
#include <vector>
#include <optional>
#include <functional>
#include "deck.h"

class CardManager {
public:
    CardManager(std::vector<Deck> decks);

    void add_deck();
    void add_card(int id, Deck& deck);
    void review_cards(int id, Deck& deck);
    void skip_day();
    void browse_cards(int id, Deck& deck);
    void delete_card(int id, Deck& deck);
    int get_int_input(const std::string& prompt) const;
    std::string get_string_input(const std::string& prompt) const;
    void display_main_menu() const;
    void display_deck_menu(int id);
    int choose_deck();

private:
    std::vector<Deck> decks_;
    std::optional<std::reference_wrapper<Deck>> find_deck_by_id(int id);
    void display_decks() const;
};