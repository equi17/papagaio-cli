#pragma once
#include <vector>
#include <functional>
#include <string>
#include "card.h"

class Deck{
public:
    Deck(std::string name);

    std::string name;
    static int next_id;

    bool add_card(const std::string& front, const std::string& back);
    bool delete_card(const int target_id);
    void review_cards(std::function<void(Card&)> review_logic);
    void browse() const;
    void skip_day();
    int get_days_skipped() const;
    int get_id() const;

private:
    int id;
    std::vector<Card> cards_;
    int day = 0;
};