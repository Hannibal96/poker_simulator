//
// Created by neria on 12/01/2024.
//

#include "Fix_Strategy_Agent.h"
#include <assert.h>

#include <utility>

Fix_Strategy_Agent::Fix_Strategy_Agent(string& name, string& path, map<State, double> strategy){
    name_ = name;
    this->strategy = std::move(strategy);
    this->path = path;
}

Action Fix_Strategy_Agent::get_action(State state) const {
    double random = global_uniform_dist(globalGen);
    double p_a = strategy.at(state);

    if (random < p_a)
        return AllIn;
    return Fold;
}

string Fix_Strategy_Agent::ToString() const {
    return "Fixed Strategy Agent " + path + "\n" + Agent::ToString();
}

string Fix_Strategy_Agent::get_entry_str(State state, int col_w) const {
    int precision = col_w - 4;

    TableEntry entry_a{state, AllIn};
    double p_a = strategy.at(state);
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << p_a;
    return " " + oss.str() + " ";
}

