#pragma once
#include <SQLiteCpp/SQLiteCpp.h>
#include <vector>
#include "deck.h"

class DatabaseManager {
public:
    DatabaseManager(const std::string& dbPath);

    void createDeckTable();
    void saveDeck(Deck& deck);
    std::vector<Deck> loadAllDecks();
    void updateDeck(const Deck& deck);
    
    void createCardTable();
    void saveCard(Card& card, const int deck_id);
    void deleteCard(const int card_id);
    void updateCard(const Card& card);

private:
    SQLite::Database db_;
};