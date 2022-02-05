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

class Bandit_table{

private:
    map<TableEntry , tuple<double, int, int> > table_;
    map<Situation, double> situation_epsilon_map, situation_final_epsilon_map, situation_decay_map, situation_tie_break_map;
    map<Situation, int> situation_decay_cycle_map;

    //double epsilon_, final_epsilon_, decay_;
    //int decay_cycle_;
    static int class_counter;

public:

    Bandit_table();
    ~Bandit_table() = default;
    Action get_action(Situation situation, Card a, Card b);
    //Action get_action(State);
    int CalcHandIdx(Card a, Card b);
    void update_table(Situation situation, Card a, Card b, Action act, double reward);
    void update_parameters(Situation situation, double start_epsilon, double final_epsilon, double decay,
                           double tie_break_epsilon, int decay_cycle);
    double calc_action_value(Situation situation, int hand, Action act, bool ucb);
    string ToString();
};

std::ostream& operator<<(std::ostream& os, Bandit_table& table);


#endif //POKER_SIMULATOR_BANDIT_TABLE_H
