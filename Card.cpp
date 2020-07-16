//
// Created by Neria on 26/06/2020.
//

#include "Card.h"

Card::Card(Value value, Suit suit) : value_(value), suit_(suit){}

Suit Card::GetSuit() {
    return suit_;
}

Value Card::GetValue() {
    return value_;
}

string Card::ToString() const {
    string card_string;
    switch(value_) {
        case 2:
            card_string += "2";
            break;
        case 3:
            card_string += "3";
            break;
        case 4:
            card_string += "4";
            break;
        case 5:
            card_string += "5";
            break;
        case 6:
            card_string += "6";
            break;
        case 7:
            card_string += "7";
            break;
        case 8:
            card_string += "8";
            break;
        case 9:
            card_string += "9";
            break;
        case 10:
            card_string += "10";
            break;
        case 11:
            card_string += "J";
            break;
        case 12:
            card_string += "Q";
            break;
        case 13:
            card_string += "K";
            break;
        case 14:
            card_string += "A";
            break;
    }
    switch (suit_){         // TODO: switch with ♥ ♦ ♠ ♣
        case Heart:
            card_string += "h";
            break;
        case Diamond:
            card_string += "d";
            break;
        case Spade:
            card_string += "s";
            break;
        case Club:
            card_string += "c";
            break;

    }
    return card_string;
}

std::ostream& operator<<(std::ostream& os, const Card& card)
{
    os << card.ToString();
    return os;
}

bool Card::operator>(const Card& card)
{
    if(*this == card)
        return false;
    if (value_ > card.value_)
        return true;
    if (value_ < card.value_)
        return false;
    //values are equal
    return  (suit_ > card.suit_);
}

bool Card::operator==(const Card& card)
{
    return card.value_ == value_ && card.suit_ == suit_ ;
}

bool Card::operator<(const Card& card)
{
    return !(*this == card || *this > card);
}
