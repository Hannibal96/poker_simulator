//
// Created by Neria on 26/06/2020.
//

#include "Deck.h"
#include "Card.h"
#include <bits/stdc++.h>
#include <ctime>

#include <random>

int Deck::curr_seed = 0;

Deck::Deck() {
    for ( int valInt = Duce; valInt != Ace+1; valInt++ ){
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

void Deck::Shuffle() {
    curr_seed ++;
    shuffle(cards.begin(), cards.end(), std::mt19937( time(nullptr) + curr_seed));
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


#include "Deck.h"
