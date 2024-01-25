//
// Created by neria on 12/01/2024.
//

#ifndef POKER_SIMULATOR_MWU_AGENT_H
#define POKER_SIMULATOR_MWU_AGENT_H
#include "Agent.h"


class MWU_Agent: public Agent{
private:
    string name_;
    double beta_;
    unsigned int T_; // number of accumulated reward rounds to update
    map<TableEntry , tuple<double, unsigned int, unsigned int, double, double> > table_; // reward, n, N, p, P

public:
    MWU_Agent(string& name, double beta, unsigned int T, double init_p);
    ~MWU_Agent() = default;
    Action get_action(State state) const override;
    void update_parameters(State state, Action action, double reward) override;
    string ToString() const override;
    string get_entry_str(State state, int col_w) const override;
};


#endif //POKER_SIMULATOR_MWU_AGENT_H
