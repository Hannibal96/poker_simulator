//
// Created by neria on 01/01/2022.
//

#ifndef POKER_SIMULATOR_BANDIT_TABLE_H
#define POKER_SIMULATOR_BANDIT_TABLE_H

#include "Card.h"
#include <map>
#include <tuple>
#include <random>
#include <cmath>       /* log */
#include "Strategy.h"

class Bandit_table{

private:
    map<TableEntry , tuple<double, int, int> > table_;
    double epsilon_;

    static int class_counter;

public:

    Bandit_table(double epsilon=0.5);
    ~Bandit_table() = default;
    Action get_action(Situation situation, int hand);
    Action get_action(State);
    void update_table(Situation situation, int hand, Action act, double reward);
    double calc_action_value(Situation situation, int hand, Action act, bool ucb);
    string ToString();
};

std::ostream& operator<<(std::ostream& os, Bandit_table& table);


#endif //POKER_SIMULATOR_BANDIT_TABLE_H
