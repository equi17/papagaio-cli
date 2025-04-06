#include "card.h"

//public functions
Card::Card(std::string front, std::string back)
    : front(front), back(back), next_review(std::time(nullptr)), id(next_id++) {};

void Card::update(bool is_correct, int days_skipped) {
    if (is_correct) advance(days_skipped);
    else            reset();
}

const int Card::get_id() const {
    return id;
}

//private functions
void Card::reset() {
    ease = std::max(1.3f, ease - 0.15f);   // penalize ease (min 1.3)
    interval = 1;
    repetitions = 0;
    next_review = std::time(nullptr) + 1440; // next review in 10 min
}
        
void Card::advance(int days_skipped) {
    // learning steps: 10m -> 1d -> graduated
    if (repetitions == 0)      interval = 10;    // 10min
    else if (repetitions == 1) interval = 1440;  // 1d (in minutes)
    else {                                       // graduated
        interval *= ease;
        ease = std::min(2.5f, ease + 0.05f);     // reward ease (max 2.5)
    }
    interval = apply_fuzz(interval);
    repetitions++;
    next_review = std::time(nullptr) + (interval * 60) + (days_skipped * 86400);
}

int Card::apply_fuzz(int minutes) const {
    float fuzz = 0.85f + (rand() % 31) * 0.01f;  // +-15% randomness
    return static_cast<int>(minutes * fuzz);
}