#include <iostream>
#include <algorithm>
#include "deck.h"

void Deck::add_card(const std::string& front, const std::string& back) {
    cards_.emplace_back(front, back, 1, std::time(nullptr));
}

void Deck::delete_card(const int target_id) {
    auto p = std::find_if(cards_.begin(), cards_.end(), [target_id](const Card& c){
        return c.get_id() == target_id;
    });
    if (p == cards_.end()) {
        std::cout << "no card with such id found!" << '\n';
    } else {
        cards_.erase(p);
        std::cout << "card deleted!" << '\n';
    }
}

void Deck::review_cards() {
    std::string answer;
    for(Card& curr: cards_) {
        time_t now = std::time(nullptr) + day * 86400;
        if(std::difftime(now, curr.next_review) >= 0) {
            std::cout << "front: " << curr.front << '\n';
            std::cout << "enter your answer:\n";
            getline(std::cin, answer);
            std::cout << "back: " << curr.back << '\n';
            std::cout << "y/n?\n";
            getline(std::cin, answer);

            if (answer == "y" || answer == "Y") {
                curr.next_review = now + (curr.group * 86400);
                ++curr.group;
            }
            else {
                curr.next_review = now + 86400;
                curr.group = 1;
            }
        }
        std::cout << "\n";
    }
}

void Deck::skip_day() {
    ++day;
}

void Deck::browse() {
    for(Card& curr: cards_) {
        const time_t review_time = curr.next_review;
        tm* local_review_time = localtime(&review_time);
        const time_t now = std::time(nullptr) + day * 86400;

        std::cout << '\n';
        std::cout << "================================\n";
        std::cout << "id: " << curr.get_id() << '\n';
        std::cout << "next review: ";
        if(std::difftime(now, curr.next_review) >= 0) {
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