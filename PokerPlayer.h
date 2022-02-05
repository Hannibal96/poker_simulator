//
// Created by Neria on 27/06/2020.
//

#ifndef POKER_SIMULATOR_POKERPLAYER_H
#define POKER_SIMULATOR_POKERPLAYER_H

#include "Strategy.h"
#include "Enums.h"
#include "Bandit_table.h"
#include "VectorStrategy.h"
#include "Q_table.h"
#include <bits/stdc++.h>
#include <iostream>
#include "hashes.h"

class PokerPlayer {
private:
    string name_;
    int id_;
    double curr_reward_;
    long double money_;

    vector<Card> holding_cards;
    uint32_t best_hand_hash;
    Position curr_position;

    Action last_action;
    Situation last_situation;

    Strategy strategy_;

    //Bandit_table bandit_table;
    //VectorStrategy vector_strategy;

public:
    PokerPlayer(string& name, int id, Strategy& strategy, Position initial_position);
    ~PokerPlayer() = default;
    void AddHoldingCards(Card card_a);
    void MockHand();
    Action GetAction(History history);
    Position GetPosition();

    Card GetHoldingCard1();
    Card GetHoldingCard2();

    double GetMoney() const;
    double GetID() const;
    uint32_t GetPlayerBestHashHand() const ;
    uint32_t EvaluateHandHash(vector<Card> community_cards);
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

#endif //POKER_SIMULATOR_POKERPLAYER_H
