//
// Created by neria on 12/01/2024.
//

#ifndef POKER_SIMULATOR_FIX_STRATEGY_AGENT_H
#define POKER_SIMULATOR_FIX_STRATEGY_AGENT_H

#include "Agent.h"

class Fix_Strategy_Agent : public Agent{
private:
    string name_;
    string path;
    map<State, double> strategy;

public:
    Fix_Strategy_Agent(string& name, string& path,  map<State, double> strategy);
    ~Fix_Strategy_Agent() = default;
    Action get_action(State state) const override;
    void update_parameters(State state, Action action, double reward) override{};
    string ToString() const override;
    string get_entry_str(State state, int col_w) const override;

};


#endif //POKER_SIMULATOR_FIX_STRATEGY_AGENT_H
