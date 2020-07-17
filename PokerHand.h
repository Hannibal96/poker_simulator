//
// Created by Neria on 27/06/2020.
//

#ifndef POKER_SIMULATOR_POKERHAND_H
#define POKER_SIMULATOR_POKERHAND_H

#include "Card.h"
#include <vector>
#include "Enums.h"

class PokerHand {
private:
    vector<Card> hand_cards;
    HandRank rank;
    vector<Value > characteristic;

    void SetRank();

public:
    PokerHand(Card card_a, Card card_b, Card card_c, Card card_d, Card card_e);
    PokerHand();
    explicit PokerHand(vector<Card>& cards);
    ~PokerHand() = default;

    HandRank GetRank();

    string ToString() const;

    bool operator>(const PokerHand& hand);
    bool operator==(const PokerHand& hand);
    bool operator<(const PokerHand& hand);

};

std::ostream& operator<<(std::ostream& os, const PokerHand& hand);

PokerHand GetBestHand(vector<Card> & seven_cards);

static bool CardGreaterThan(Card c1, Card c2);

#endif //POKER_SIMULATOR_POKERHAND_H
