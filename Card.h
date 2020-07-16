//
// Created by Neria on 26/06/2020.
//

#ifndef POKER_SIMULATOR_CARD_H
#define POKER_SIMULATOR_CARD_H

#include "Enums.h"
#include <iostream>

using namespace std;

class Card {
private:
    Value value_;
    Suit suit_;

public:
    Card(Value value, Suit suit);
    ~Card() = default;

    Value GetValue();
    Suit GetSuit();

    string ToString() const;

    bool operator>(const Card& card);
    bool operator==(const Card& card);
    bool operator<(const Card& card);

};

std::ostream& operator<<(std::ostream& os, const Card& card);


#endif //POKER_SIMULATOR_CARD_H
