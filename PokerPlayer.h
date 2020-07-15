//
// Created by Neria on 27/06/2020.
//

#ifndef POKER_SIMULATOR_POKERPLAYER_H
#define POKER_SIMULATOR_POKERPLAYER_H

#include "Strategy.h"
#include "PokerHand.h"
#include "Enums.h"
#include <bits/stdc++.h>
#include <iostream>

class PokerPlayer {
private:
    string name_;
    int id_;
    double money_;
    Strategy strategy_;

    vector<Card> holding_cards;
    PokerHand best_hand;
    Position curr_position;

    Action last_action;

public:
    PokerPlayer(string& name, int id, Strategy& strategy, Position initial_position);
    ~PokerPlayer() = default;
    void AddHoldingCards(Card card_a);
    void AddHoldingCards(Card card_a, Card card_b);
    void MockHand();
    Action GetAction(PreviousAction previous_action);
    Action GetAction() const;
    Position GetPosition();
    double GetMoney() const;
    double GetID() const;
    PokerHand GetPlayerBestHand() const ;
    PokerHand EvaluateHand(vector<Card> community_cards);
    void UpdatePosition();
    void UpdateMoney(double delta);
    void UnSetAction();

    string ToString() const;
    bool operator>(const PokerPlayer& player);
    bool operator==(const PokerPlayer& player);

};

std::ostream& operator<<(std::ostream& os, const PokerHand& hand);

#endif //POKER_SIMULATOR_POKERPLAYER_H
