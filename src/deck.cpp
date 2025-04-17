#include <iostream>
#include <algorithm>
#include <functional>
#include "deck.h"

Deck::Deck(const std::string& name): name(name), id(0), day(0) {}

bool Deck::add_card(const std::string& front, const std::string& back) {
    auto it = std::find_if(cards_.begin(), cards_.end(), [&front](const Card& card){ 
        return card.front == front; 
    });

    if (it != cards_.end()) {
        return true;
    }
    cards_.emplace_back(front, back, id);
    return false;
}

void Deck::add_card(const Card& card) {
    cards_.push_back(card);
}

bool Deck::delete_card(const int target_id) {
    auto it = std::find_if(cards_.begin(), cards_.end(), 
        [target_id](const Card& c){ return c.get_id() == target_id; }
    );

    if (it != cards_.end()) {
        cards_.erase(it);
        return true;
    }
    return false;
}

void Deck::review_cards(std::function<void(Card&)> review_logic) {
    for(Card& card: cards_) {
        time_t now = std::time(nullptr) + day * 86400;
        time_t next_review = card.next_review;
        if(std::difftime(now, next_review) >= 0) {
            review_logic(card);
        }
    }
}

void Deck::skip_day() {
    ++day;
}

int Deck::get_days_skipped() const {
    return day;
}

void Deck::browse() const {
    for(const Card& curr: cards_) {
        const time_t review_time = curr.next_review;
        tm* local_review_time = localtime(&review_time);
        const time_t now = std::time(nullptr) + day * 86400;

        std::cout << '\n';
        std::cout << "================================\n";
        std::cout << "id: " << curr.get_id() << '\n';
        std::cout << "next review: ";
        if(std::difftime(now, review_time) >= 0) {
            std::cout << "ready for review!\n";
        } else {
            char buf[80];
            strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M:%S", local_review_time);
            std::cout << buf << "\n";
        }
        std::cout << "front: " << curr.front << '\n';
        std::cout << "back: " << curr.back << '\n';
        std::cout << "================================";
        std::cout << '\n';
    }
}

int Deck::get_id() const {
    return id;
}

void Deck::set_days_skipped(int days_skipped) {
    day = days_skipped;
}

void Deck::set_id(int deck_id) {
    id = deck_id;
}

bool Deck::card_exists(const std::string& front) const {
    return std::any_of(cards_.begin(), cards_.end(),
        [&front](const Card& c) { return c.front == front; });
}