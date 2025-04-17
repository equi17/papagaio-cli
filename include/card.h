#pragma once
#include <string>
#include <ctime>

class Card {
    public:
        std::string front;
        std::string back;
        std::time_t next_review;
    
        Card(const std::string& front, const std::string& back, int deck_id);

        void update(bool is_correct, int days_skipped);
        const int get_id() const;
        const int get_deck_id() const;
        const int get_interval() const;
        const float get_ease() const;
        const int get_repetitions() const;
        void set_id(int i);
        void set_repetitions(int r);
        void set_deck_id(int di);
        void set_interval(int i);
        void set_ease(int e);

    private:
        int id = 0;
        int deck_id;
        int interval = 1;    // interval until next review in mins (applies to graduated cards)
        float ease = 2.5f;   // multiplier (1.3=hard, 2.5=easy)
        int repetitions = 0; // keeps track of successful reviews

        void reset();
        void advance(int days_skipped);
        int apply_fuzz(int minutes) const;
};