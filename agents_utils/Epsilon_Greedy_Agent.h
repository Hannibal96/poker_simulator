//
// Created by neria on 08/01/2024.
//

#ifndef POKER_SIMULATOR_EPSILON_GREEDY_AGENT_H
#define POKER_SIMULATOR_EPSILON_GREEDY_AGENT_H

#include "Agent.h"


class Epsilon_Greedy_Agent: public Agent {
private:
    string name_;
    double min_epsilon_, decay_rate_;
    map<TableEntry, tuple<double, double, unsigned int>> table_; // epsilon, total_reward, n

public:
    Epsilon_Greedy_Agent(string& name, double init_epsilon, double min_epsilon, double decay_rate);
    ~Epsilon_Greedy_Agent() = default;
    Action get_action(State state) const override;
    void update_parameters(State state, Action action, double reward) override;
    string ToString() const override;
    string get_entry_str(State state, int col_w) const override ;
};


#endif //POKER_SIMULATOR_EPSILON_GREEDY_AGENT_H
