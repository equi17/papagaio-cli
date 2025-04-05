#pragma once
#include <ctime>
#include <vector>
#include "card.h"

class Deck{
    std::vector<Card> cards;
    int day = 0;
public:
    void add_card(const std::string& front, const std::string& back);
    void review_cards();
    void browse();
    void skip_day();
};