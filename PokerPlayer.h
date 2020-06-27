//
// Created by Neria on 27/06/2020.
//

#ifndef POKER_SIMULATOR_POKERPLAYER_H
#define POKER_SIMULATOR_POKERPLAYER_H

#include "Strategy.h"
#include "PokerHand.h"


class PokerPlayer {
private:
    string name_;
    int money_, id_;
    Strategy strategy_;
    vector<Card> holding_cards;

    PokerHand best_hand;

public:
    PokerPlayer(string& name, int id, Strategy& strategy);
    ~PokerPlayer() = default;
    void AddHoldingCards(Card card_a);
    void AddHoldingCards(Card card_a, Card card_b);
    void EvaluateHand(vector<Card> community_cards);

};


#endif //POKER_SIMULATOR_POKERPLAYER_H
