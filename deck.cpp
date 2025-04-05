#include <iostream>
#include "deck.h"

void Deck::add_card(const std::string& front, const std::string& back) {
    cards.emplace_back(front, back, 1, std::time(nullptr));
}

void Deck::review_cards() {
    time_t now = std::time(nullptr) + day * 86400;
    std::string answer;
    for(Card& curr: cards) {
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

void Deck::browse() {}