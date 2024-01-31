//
// Created by neria on 08/01/2024.
//

#include "Bandit_Agent.h"


Bandit_Agent::Bandit_Agent(std::string &name) {
    name_ = name;
    for (int situation = 0; situation <= BB_CO_DE_SB; situation++) {
        for (int hand = 0; hand < 169; hand++) {
            for (int act = Fold; act <= AllIn; act++) {
                State state{static_cast<Situation>(situation), hand};
                TableEntry entry{state, static_cast<Action>(act)};
                table_[entry] = tuple<double, int, int>{0.0, 0, 0};
            }
        }
    }
}


Action Bandit_Agent::get_action(State state) const {

    TableEntry entry_f{state, Fold};
    TableEntry entry_a{state, AllIn};

    auto f_data = table_.at(entry_f);
    auto a_data = table_.at(entry_a);

    double ucb_a = calc_ucb(a_data);
    double ucb_f = calc_ucb(f_data);

    if (ucb_a == ucb_f){
        double random = global_uniform_dist(globalGen);
        if (random < 0.5)
            return AllIn;
        return Fold;
    }

    if (ucb_a > ucb_f)
        return AllIn;
    return Fold;

}

void Bandit_Agent::update_parameters(State state, Action action, double reward) {
    TableEntry entry{state, action}, next_entry;
    if(action == AllIn)
        next_entry = TableEntry{state, Fold};
    else
        next_entry = TableEntry{state, AllIn};
    get<0>(table_[entry]) += reward;
    get<1>(table_[entry]) += 1;
    get<2>(table_[entry]) += 1;
    get<1>(table_[next_entry]) += 1;
}


string Bandit_Agent::get_entry_str(State state, int col_w) const{
    int precision = col_w - 6;

    TableEntry entry_a{state, AllIn};
    TableEntry entry_f{state, Fold};

    auto f_data = table_.at(entry_f);
    auto a_data = table_.at(entry_a);

    double f_reward = get<0>(f_data) / (get<2>(f_data) + !get<2>(f_data));
    double a_reward = get<0>(a_data) / (get<2>(a_data) + !get<2>(a_data));
    double diff = abs(a_reward - f_reward);

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << diff;

    if(a_reward >= f_reward){
        return " A " + oss.str() + " ";
    }
    return " F " + oss.str() + " ";
}

string Bandit_Agent::ToString() const {
    return "Bandit Agent \n" + Agent::ToString();
}