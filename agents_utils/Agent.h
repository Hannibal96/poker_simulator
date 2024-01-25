//
// Created by neria on 08/01/2024.
//

#ifndef POKER_SIMULATOR_AGENT_H
#define POKER_SIMULATOR_AGENT_H

#include "../poker_utils/Enums.h"
#include "../deck_utils/Card.h"
#include <map>
#include <tuple>
#include <cmath>
#include <iostream>
#include <sstream>
#include <iomanip>

class Agent {
private:
    string name;

public:
    virtual Action get_action(State state) const = 0;
    virtual void update_parameters(State state, Action action, double reward) = 0; // TODO: maybe add next_state as input
    virtual string get_name(){
        return name;
    }
    virtual string get_entry_str(State state, int col_w) const = 0;

    virtual string ToString() const;
    static string idx_to_hand_string(int idx){
        int M = int(sqrt(idx));
        idx -= M * M;
        int m = idx / 2;
        idx -= 2 * m;
        bool suit = idx %2;
        M += 2;
        m += 2;
        string hand = card_val_str.at(M) + card_val_str.at(m);
        if (suit)
            hand += "s";
        else
            hand += "o";
        return hand;
    }
};

std::ostream& operator<<(std::ostream& os, const Agent& agent);




#endif //POKER_SIMULATOR_AGENT_H
