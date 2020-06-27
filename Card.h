//
// Created by Neria on 26/06/2020.
//

#ifndef POKER_SIMULATOR_CARD_H
#define POKER_SIMULATOR_CARD_H
#include <iostream>

using namespace std;

enum Suit{Heart , Diamond, Spade, Club};
enum Value{Duce = 2, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace};

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

};

std::ostream& operator<<(std::ostream& os, const Card& card);


#endif //POKER_SIMULATOR_CARD_H
