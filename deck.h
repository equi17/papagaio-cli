#pragma once
#include <ctime>
#include <vector>
#include "card.h"

class Deck{
public:
    void add_card(const std::string& front, const std::string& back);
    void delete_card(const int target_id);
    void review_cards();
    void browse();
    void skip_day();
private:
    std::vector<Card> cards_;
    int day = 0;
};