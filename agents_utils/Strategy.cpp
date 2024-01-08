//
// Created by Neria on 27/06/2020.
//

#include "Strategy.h"
#include <iostream>

Strategy::Strategy() {
    bandit = Bandit_table();
    vectors_strategy = map<Situation, VectorStrategy>();
}

void Strategy::UpdateBandit(Situation situation, double start_epsilon, double final_epsilon, double decay,
                            double tie_break_epsilon, int decay_cycle){
    bandit.update_parameters(situation, start_epsilon, final_epsilon, decay, tie_break_epsilon, decay_cycle);
}

void Strategy::UpdateVector(Situation situation, VectorStrategy& vector){
    vectors_strategy[situation] = vector;
}

string Strategy::GetName() {
    return name_;
}

Action Strategy::GetAction(Situation situation, Card a, Card b) {
    if(vectors_strategy.find(situation) == vectors_strategy.end()){  // Use Bandit
        return bandit.get_action(situation, a, b);
    }
    return vectors_strategy[situation].GetAction(a, b);
}

void Strategy::UpdateParameters(Situation situation, Card a, Card b, Action act, double reward) {
    bandit.update_table(situation, a, b, act, reward);
}

Bandit_table Strategy::get_bandit() {
    return bandit;
}
