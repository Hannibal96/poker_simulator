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
    return card_val_str.at(value_) + card_suit_str.at(suit_);
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
