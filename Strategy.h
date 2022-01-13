//
// Created by Neria on 27/06/2020.
//

#ifndef POKER_SIMULATOR_STRATEGY_H
#define POKER_SIMULATOR_STRATEGY_H

#include <iostream>
#include <vector>
#include "Card.h"
#include "Enums.h"

using namespace std;


class Strategy {
private:
    string name_;

public:
    Strategy() = default;
    ~Strategy() = default;

    Action GetAction();
    void UpdateParameters();
    string GetName();

};


#endif //POKER_SIMULATOR_STRATEGY_H
