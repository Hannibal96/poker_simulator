//
// Created by Neria on 27/06/2020.
//

#ifndef POKER_SIMULATOR_POKERPLAYER_H
#define POKER_SIMULATOR_POKERPLAYER_H

#include "Strategy.h"
#include "PokerHand.h"
#include "Enums.h"
#include "Bandit_table.h"
#include "VectorStrategy.h"
#include "Q_table.h"
#include <bits/stdc++.h>
#include <iostream>

class PokerPlayer {
private:
    string name_;
    int id_;
    double money_, curr_reward_;

    vector<Card> holding_cards;
    PokerHand best_hand;
    Position curr_position;

    Action last_action;
    Situation last_situation;

    Bandit_table bandit_table;
    VectorStrategy vector_strategy;

public:
    PokerPlayer(string& name, int id, VectorStrategy& strategy, Position initial_position);
    ~PokerPlayer() = default;
    void AddHoldingCards(Card card_a);
    void MockHand();
    Action GetAction(History history);
    Position GetPosition();
    double GetMoney() const;
    double GetID() const;
    PokerHand GetPlayerBestHand() const ;
    PokerHand EvaluateHand(vector<Card> community_cards);
    void UpdatePosition();
    void UpdateMoney(double delta);
    void UnSetAction();
    void ResetReward();
    void UpdateTable();


    string GetStrategyName();

    string ToString() ;
    bool operator>(const PokerPlayer& player);
    bool operator==(const PokerPlayer& player);
    bool operator<(const PokerPlayer& player);

};

std::ostream& operator<<(std::ostream& os, PokerHand& hand);

#endif //POKER_SIMULATOR_POKERPLAYER_H
