//
// Created by neria on 12/01/2024.
//

#ifndef POKER_SIMULATOR_FIX_STRATEGY_AGENT_H
#define POKER_SIMULATOR_FIX_STRATEGY_AGENT_H

#include "Agent.h"

class Fix_Strategy_Agent : public Agent{
private:
    string name_;
    map<State, double> strategy;

public:
    //Fix_Strategy_Agent(string& name, map<State, double> strategy); // TODO: decide if define by table or json file path
    //~Fix_Strategy_Agent() = default;
    Action get_action(State state) const override = 0; // TODO: implement
    void update_parameters(State state, Action action, double reward) override = 0;  // TODO: implement
    string ToString() const override = 0; // TODO: implement
    string get_entry_str(State state, int col_w) const override = 0; // TODO: implement

};


#endif //POKER_SIMULATOR_FIX_STRATEGY_AGENT_H
