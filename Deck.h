//
// Created by Neria on 26/06/2020.
//

#ifndef POKER_SIMULATOR_DECK_H
#define POKER_SIMULATOR_DECK_H

using namespace std;

#include <vector>
#include "Card.h"

class Deck {
public:
    Deck();
    ~Deck() = default;

    Card DealCard();
    void Shuffle();

    string ToString() const;
    static int curr_seed;

private:
    vector<Card> cards;
};

std::ostream& operator<<(std::ostream& os, const Deck& deck);


#endif //POKER_SIMULATOR_DECK_H
