#include <algorithm>
#include <random>
#include "card.h"

//public functions
Card::Card(const std::string& front, const std::string& back, int deck_id)
        : front(front), back(back), deck_id(deck_id), 
          next_review(time(nullptr)), id(0), ease(2.5), 
          repetitions(0), interval(1) {}

void Card::update(bool is_correct, int days_skipped) {
    if (is_correct) advance(days_skipped);
    else            reset();
}

const int Card::get_id() const { return id; }
const int Card::get_deck_id() const { return deck_id; }
const int Card::get_interval() const { return interval; }
const float Card::get_ease() const { return ease; }
const int Card::get_repetitions() const { return repetitions; }

void Card::set_id(int i) { id = i; }
void Card::set_repetitions(int r) { repetitions = r; }
void Card::set_deck_id(int di) { deck_id = di; }
void Card::set_interval(int i) { interval = i; }
void Card::set_ease(int e) { ease = e; }

//private functions
void Card::reset() {
    ease = std::max(1.3f, ease - 0.15f);   // penalize ease (min 1.3)
    interval = 1;
    repetitions = 0;
    next_review = std::time(nullptr) + 10; // next review in 10 min
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

template <typename T>
T lerp(T a, T b, float t) {
    return a + t * (b - a);
}

float smootherstep(float t) {
    t = std::clamp(t, 0.0f, 1.0f);
    return t * t * t * (t * (6.0f * t - 15.0f) + 10.0f);
}

// the delay amount increases more smoothly as intervals increase
int Card::apply_fuzz(int minutes) const {
    const float MIN_DAYS = 1.0f;   // fuzz starts at 1 days
    const float MAX_DAYS = 30.0f;  // fuzz maxes out at 30 days
    const float MIN_FUZZ = 0.0f;  // 1% fuzz for small intervals
    const float MAX_FUZZ = 0.15f;  // 15% fuzz for large intervals

    float interval_days = minutes / 1440.0f;

    float t = (interval_days - MIN_DAYS) / (MAX_DAYS - MIN_DAYS);
    t = smootherstep(t);

    // fuzz factor between MIN_FUZZ and MAX_FUZZ
    float fuzz_factor = lerp(MIN_FUZZ, MAX_FUZZ, t);

    // generate random fuzz in [-fuzz_factor, +fuzz_factor]
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> dist(-fuzz_factor, fuzz_factor);
    float fuzz = 1.0f + dist(gen); // 1.0 +- fuzz_factor

    return static_cast<int>(std::round(minutes * fuzz));
}