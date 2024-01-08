//
// Created by neria on 04/01/2022.
//

#ifndef POKER_SIMULATOR_VECTORSTRATEGY_H
#define POKER_SIMULATOR_VECTORSTRATEGY_H

#include <iostream>
#include <vector>
#include "../deck_utils/Card.h"
#include "../poker_utils/Enums.h"

using namespace std;


class VectorStrategy{
private:
    string name_;
    vector<int> unsuited_strategy_;
    vector<int> suited_strategy_;

public:
    VectorStrategy(vector<int>& unsuited_strategy, vector<int>& suited_strategy, string& name);
    VectorStrategy() = default;
    ~VectorStrategy() = default;

    Action GetAction(Card card_a, Card card_b);
    string GetName();

};



#endif //POKER_SIMULATOR_VECTORSTRATEGY_H
