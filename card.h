#pragma once
#include <string>
#include <ctime>

class Card {
    public:
        std::string front;
        std::string back;
        std::time_t next_review;
    
        static int next_id;
    
        Card(std::string front, std::string back);

        void update(bool is_correct, int days_skipped);
        const int get_id() const;

    private:
        int id;
        int interval = 1;    // interval until next review in mins (applies to graduated cards)
        float ease = 2.5f;   // multiplier (1.3=hard, 2.5=easy)
        int repetitions = 0; // keeps track of successful reviews

        void reset();
        void advance(int days_skipped);
        int apply_fuzz(int minutes) const;
};