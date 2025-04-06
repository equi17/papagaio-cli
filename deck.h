#pragma once
#include <ctime>
#include <vector>
#include <functional>
#include "card.h"

class Deck{
public:
    void add_card(const std::string& front, const std::string& back);
    bool delete_card(const int target_id);
    void review_cards(std::function<void(Card&)> review_logic);
    void browse() const;
    void skip_day();
    int get_days_skipped() const;

private:
    std::vector<Card> cards_;
    int day = 0;
};