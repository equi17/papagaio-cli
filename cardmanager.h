#pragma once
#include "deck.h"
#include <string>

class CardManager {
public:
    CardManager(Deck& deck);

    void add_card();
    void review_cards();
    void skip_day();
    void browse_cards();
    void delete_card();
    int get_int_input(const std::string& prompt);
    std::string get_string_input(const std::string& prompt);
    void display_menu() const;

private:
    Deck& deck_;
};