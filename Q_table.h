//
// Created by neria on 28/12/2021.
//

#ifndef POKER_SIMULATOR_Q_TABLE_H
#define POKER_SIMULATOR_Q_TABLE_H

#include "Card.h"
#include <map>
#include <tuple>
#include <random>


class Q_table{
private:
    map<TableEntry , double > table_;
    double alpha_,  epsilon_;
    default_random_engine generator;

public:
    Q_table(double alpha, double epsilon);
    ~Q_table() = default;
    Action get_action(Situation situation, int hand);
    void update_table(Situation situation, int hand, Action act, double reward);

};


#endif //POKER_SIMULATOR_Q_TABLE_H
