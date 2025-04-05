#pragma once
#include <string>
#include <ctime>

class Card {
    private:
        int id;
    public:
        std::string front;
        std::string back;
        int group;
        std::time_t next_review;
    
        static int next_id;
    
        Card(std::string front, std::string back, int group, std::time_t next_review = std::time(nullptr))
            : id(next_id++), front(front), back(back), group(group), next_review(next_review) {}
};