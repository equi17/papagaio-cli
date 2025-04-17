#pragma once
#include <vector>
#include <functional>
#include <string>
#include "card.h"

class Deck{
public:
    Deck(const std::string& name);

    std::string name;

    bool add_card(const std::string& front, const std::string& back);
    void add_card(const Card& card);
    bool delete_card(const int target_id);
    void review_cards(std::function<void(Card&)> review_logic);
    void browse() const;
    void skip_day();
    int get_days_skipped() const;
    int get_id() const;
    void set_days_skipped(int days);
    void set_id(int deck_id);
    bool card_exists(const std::string& front) const;

private:
    int id = 0;
    std::vector<Card> cards_;
    int day = 0;
};