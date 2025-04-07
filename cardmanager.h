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
    void add_card(int id);
    void review_cards(int id);
    void skip_day();
    void browse_cards(int id);
    void delete_card(int id);
    int get_int_input(const std::string& prompt) const;
    std::string get_string_input(const std::string& prompt) const;
    void display_menu() const;
    int choose_deck() const;

private:
    std::vector<Deck> decks_;
    std::optional<std::reference_wrapper<Deck>> find_deck_by_id(int id);
    void display_decks() const;
};