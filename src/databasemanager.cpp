#include "databasemanager.h"

DatabaseManager::DatabaseManager(const std::string& dbPath)
    : db_(dbPath, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE) {
    createDeckTable();
    createCardTable(); 
}

void DatabaseManager::createDeckTable() {
    db_.exec(R"(
        CREATE TABLE IF NOT EXISTS decks (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL UNIQUE,
            days_skipped INTEGER DEFAULT 0,
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        )
    )");
}

void DatabaseManager::saveDeck(const Deck& deck) {
    SQLite::Statement query(db_, 
        "INSERT OR REPLACE INTO decks (id, name, days_skipped) VALUES (?, ?, ?)");
    
    query.bind(1, deck.get_id());
    query.bind(2, deck.name);
    query.bind(3, deck.get_days_skipped());
    
    query.exec();
}

void DatabaseManager::updateDeck(const Deck& deck) {
    SQLite::Statement query(db_,
        "UPDATE decks "
        "SET days_skipped = ? "
        "WHERE id = ?");

    query.bind(1, deck.get_days_skipped());
    query.bind(2, deck.get_id());

    query.exec();
}

std::vector<Deck> DatabaseManager::loadAllDecks() {
    std::vector<Deck> decks;

    SQLite::Statement query(db_, 
        "SELECT id, name, days_skipped FROM decks");

    while (query.executeStep()) {
        int id = query.getColumn(0);
        std::string name = query.getColumn(1);
        int daysSkipped = query.getColumn(2);
            
        Deck deck(name);
        deck.set_id(id);
        deck.set_days_skipped(daysSkipped);
            
        SQLite::Statement cardQuery(db_, 
            "SELECT id, front, back, next_review, ease, repetition, interval "
            "FROM cards WHERE deck_id = ?");
        cardQuery.bind(1, id);
            
        while (cardQuery.executeStep()) {
            Card card(
                cardQuery.getColumn(1),
                cardQuery.getColumn(2),
                id
            );
            card.next_review = cardQuery.getColumn(3);
            card.set_ease(cardQuery.getColumn(4));
            card.set_repetitions(cardQuery.getColumn(5));
            card.set_interval(cardQuery.getColumn(6));
            card.set_id(cardQuery.getColumn(0));
                
            deck.add_card(card);  // You'll need to modify your Deck class to support this
        }
            
        decks.push_back(deck);
    }
        
    return decks;
}
    
void DatabaseManager::createCardTable() {
    db_.exec(R"(
        CREATE TABLE IF NOT EXISTS cards (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            deck_id INTEGER NOT NULL,
            front TEXT NOT NULL,
            back TEXT NOT NULL,
            next_review TIMESTAMP NOT NULL,
            ease REAL DEFAULT 2.5,
            repetition INTEGER DEFAULT 0,
            interval INTEGER DEFAULT 1,
            UNIQUE(deck_id, front),
            FOREIGN KEY (deck_id) REFERENCES decks(id) ON DELETE CASCADE
        )
    )");
}

void DatabaseManager::saveCard(Card& card, const int deck_id) {
    SQLite::Statement query(db_, 
        "INSERT INTO cards "
        "(deck_id, front, back, next_review, ease, repetition, interval) "
        "VALUES (?, ?, ?, ?, ?, ?, ?)");
    
    query.bind(1, deck_id);
    query.bind(2, card.front);
    query.bind(3, card.back);
    query.bind(4, static_cast<int64_t>(card.next_review));
    query.bind(5, card.get_ease());
    query.bind(6, card.get_repetitions());
    query.bind(7, card.get_interval());
    
    query.exec();
    
    if (card.get_id() == 0) {
        card.set_id(db_.getLastInsertRowid());
    }
}

void DatabaseManager::deleteCard(const int card_id) {
    SQLite::Statement query(db_,
        "DELETE FROM cards WHERE id = ?");
    query.bind(1, card_id);
    if (query.exec() == 0) {
        throw std::runtime_error("No card found with ID " + std::to_string(card_id));
    }
}

void DatabaseManager::updateCard(const Card& card) {
    SQLite::Statement query(db_,
        "UPDATE cards "
        "SET next_review = ?, ease = ?, repetition = ?, interval = ? "
        "WHERE id = ?");

    query.bind(1, card.next_review);
    query.bind(2, card.get_ease());
    query.bind(3, card.get_repetitions());
    query.bind(4, card.get_interval());
    query.bind(5, card.get_id());

    query.exec();
}