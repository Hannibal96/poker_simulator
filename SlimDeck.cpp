//
// Created by neria on 12/01/2022.
//

#include "SlimDeck.h"
#include <ctime>

#include <random>
#include <bits/stdc++.h>

int SlimDeck::curr_seed = 0;

SlimDeck::SlimDeck() {
    for ( uint8_t suit = 0; suit < 4 ; suit++ ){
        for ( uint8_t val = 0; val < 13 ; val++ ){
            cards.push_back((val << 2) | suit);
        }
    }
}

SlimCard SlimDeck::DealCard() {
    SlimCard dealt_card = cards.front();
    cards.erase(cards.begin());
    return dealt_card;
}

void SlimDeck::Shuffle() {
    curr_seed ++;
    shuffle(cards.begin(), cards.end(), std::mt19937( time(nullptr) + curr_seed));
}


string SlimDeck::ToString() const {     // TODO: fix visabillity, add delimiters such as | and allignment

    map<uint8_t, string> suit_2_string = {
            {0, "h"},
            {1, "d"},
            {2, "s"},
            {3, "c"}             };

    map<uint8_t, string> val_2_string = {
            {0, "2"},
            {1, "3"},
            {2, "4"},
            {3, "5"},
            {4, "6"},
            {5, "7"},
            {6, "8"},
            {7, "9"},
            {8, "T"},
            {9, "J"},
            {10, "Q"},
            {11, "K"},
            {12, "A"}             };

    string deck_string;
    int counter = 0;
    for (auto & card : cards) {
        deck_string += val_2_string[card >> 2] + suit_2_string[card % 4] + "\t";
        counter++;
        if (counter == 10){
            counter = 0;
            deck_string += "\n";
        }
    }
    return deck_string;
}

std::ostream& operator<<(std::ostream& os, const SlimDeck& deck) {
    os << deck.ToString();
    return os;
}

