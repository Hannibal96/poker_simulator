//
// Created by neria on 08/01/2024.
//

#ifndef POKER_SIMULATOR_BANDIT_AGENT_H
#define POKER_SIMULATOR_BANDIT_AGENT_H
#include "Agent.h"


class Bandit_Agent: public Agent{
private:
    string name_;
    map<TableEntry , tuple<double, unsigned int, unsigned int> > table_; // reward, N, n // TODO: check unordered maps for better performance

    static double calc_ucb(tuple<double, int, int> entry_data) {
        double total_reward = get<0>(entry_data);
        int N = get<1>(entry_data);
        int n = get<2>(entry_data);
        return total_reward / (n + !n) + sqrt(2 * log(N + !N) / (n + !n));
    }

public:
    explicit Bandit_Agent(string& name);
    ~Bandit_Agent() = default;
    Action get_action(State state) const override;
    void update_parameters(State state, Action action, double reward) override;
    string get_entry_str(State state, int col_w) const override;
    string ToString() const override;

};


#endif //POKER_SIMULATOR_BANDIT_AGENT_H
