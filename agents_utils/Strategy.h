//
// Created by Neria on 27/06/2020.
//

#ifndef POKER_SIMULATOR_STRATEGY_H
#define POKER_SIMULATOR_STRATEGY_H

#include <iostream>
#include <vector>
#include "../deck_utils/Card.h"
#include "../poker_utils/Enums.h"
#include "Bandit_table.h"
#include <bits/stdc++.h>
#include "VectorStrategy.h"

using namespace std;


class Strategy {
private:
    string name_;
    map<Situation, VectorStrategy> vectors_strategy;
    Bandit_table bandit;

public:
    Strategy();
    ~Strategy() = default;

    void UpdateBandit(Situation situation, double start_epsilon, double final_epsilon, double decay,
                      double tie_break_epsilon, int decay_cycle);
    void UpdateVector(Situation situation, VectorStrategy& vector);

    Bandit_table get_bandit();

    Action GetAction(Situation situation, Card a, Card b);
    void UpdateParameters(Situation situation, Card a, Card b, Action act, double reward);
    string GetName();

};


#endif //POKER_SIMULATOR_STRATEGY_H
