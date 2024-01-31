//
// Created by Neria on 26/06/2020.
//

#include "Deck.h"
#include <bits/stdc++.h>
#include <ctime>


Deck::Deck() {
    for ( int valInt = Deuce; valInt != Ace+1; valInt++ ){
        auto val = static_cast<Value >(valInt);
        for ( int suitInt = Heart; suitInt != Club+1; suitInt++ ){
            auto suit = static_cast<Suit >(suitInt);
            Card new_card = Card(val, suit);
            cards.push_back(new_card);
        }
    }
}

Card Deck::DealCard() {
    Card dealt_card = cards.front();
    cards.erase(cards.begin());
    return dealt_card;
}

void Deck::DisCard(Card card){
    auto pos = find(cards.begin(), cards.end(), card);
    if(pos == cards.end())
        throw ;
    cards.erase(pos);
}

void Deck::Shuffle() {
    // / static std::random_device rd;  // Will be used to obtain a seed for the random number engine
    //static std::mt19937 g(rd()); // Standard mersenne_twister_engine seeded with rd()
    // / static std::mt19937 g(global_seed);
    //shuffle(cards.begin(), cards.end(), std::mt19937( time(nullptr) + dis(gen)));
    shuffle(cards.begin(), cards.end(), globalGen);
}

string Deck::ToString() const {     // TODO: fix visabillity, add delimiters such as | and allignment
    string deck_string;
    int counter = 0;
    for (auto & card : cards) {
        deck_string += card.ToString() + "\t";
        counter++;
        if (counter == 10){
            counter = 0;
            deck_string += "\n";
        }
    }
    return deck_string;
}

std::ostream& operator<<(std::ostream& os, const Deck& deck) {
    os << deck.ToString();
    return os;
}


