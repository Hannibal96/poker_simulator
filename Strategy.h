//
// Created by Neria on 27/06/2020.
//

#ifndef POKER_SIMULATOR_STRATEGY_H
#define POKER_SIMULATOR_STRATEGY_H

#include <iostream>
#include <vector>
#include "Card.h"

using namespace std;

enum Action{Fold, AllIn};

class Strategy {
private:
    string name_;
    vector<int> unsuited_strategy_;
    vector<int> suited_strategy_;

public:
    Strategy(vector<int>& unsuited_strategy, vector<int>& suited_strategy, string& name);
    ~Strategy() = default;

    Action GetAction(Card card_a, Card card_b);

};


#endif //POKER_SIMULATOR_STRATEGY_H
