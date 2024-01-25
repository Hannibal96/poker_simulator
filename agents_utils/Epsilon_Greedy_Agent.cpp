//
// Created by neria on 08/01/2024.
//

#include "Epsilon_Greedy_Agent.h"

Epsilon_Greedy_Agent::Epsilon_Greedy_Agent(std::string &name, double init_epsilon, double min_epsilon,
                                           double decay_rate) {
    name_ = name;
    for (int situation = 0; situation <= BB_CO_DE_SB; situation++) {
        for (int hand = 0; hand < 169; hand++) {
            for (int act = Fold; act <= AllIn; act++) {
                State state{static_cast<Situation>(situation), hand};
                TableEntry entry{state, static_cast<Action>(act)};
                table_[entry] = tuple<double, double, unsigned int>{init_epsilon, 0.0, 0};
            }
        }
    }
    min_epsilon_ = min_epsilon;
    decay_rate_ = decay_rate;
    // TODO: random stuff
}

Action Epsilon_Greedy_Agent::get_action(State state) const {

    TableEntry entry_f{state, Fold}, entry_a{state, AllIn};
    double epsilon = get<0>(table_.at(entry_a));
    double random = (double) rand() / (RAND_MAX);

    if (random < epsilon){ // exploration
        random = (double) rand() / (RAND_MAX);
        if (random < 0.5)
            return Fold;
        return AllIn;
    }
    // exploitation
    double total_reward_a = get<1>(table_.at(entry_a));
    unsigned int n_a = get<2>(table_.at(entry_a));
    double total_reward_f = get<1>(table_.at(entry_f));
    unsigned int n_f = get<2>(table_.at(entry_f));

    double reward_a = total_reward_a / (n_a + int (!n_a));
    double reward_f = total_reward_f / (n_f + int (!n_f));

    if (reward_a == reward_f){
        random = (double) rand() / (RAND_MAX);
        if (random < 0.5)
            return Fold;
        return AllIn;
    }
    if (reward_a > reward_f)
        return AllIn;
    return Fold;
}

void Epsilon_Greedy_Agent::update_parameters(State state, Action action, double reward) {
    TableEntry entry_f{state, Fold}, entry_a{state, AllIn};
    double new_epsilon = get<0>(table_.at(entry_f)) * decay_rate_;
    new_epsilon = max(new_epsilon, min_epsilon_);

    get<0>(table_.at(entry_a)) = new_epsilon;
    get<0>(table_.at(entry_f)) = new_epsilon;

    if(action == Fold){
        get<1>(table_.at(entry_f)) += reward;
        get<2>(table_.at(entry_f)) += 1;
    }
    else {
        get<1>(table_.at(entry_a)) += reward;
        get<2>(table_.at(entry_a)) += 1;
    }
}


string Epsilon_Greedy_Agent::ToString() const{
    return "EG Agent \n" + Agent::ToString();
}


string Epsilon_Greedy_Agent::get_entry_str(State state, int col_w) const{
    int precision = col_w - 6;

    TableEntry entry_a{state, AllIn};
    TableEntry entry_f{state, Fold};

    auto f_data = table_.at(entry_f);
    auto a_data = table_.at(entry_a);

    double f_reward = get<1>(f_data) / (get<2>(f_data) + !get<2>(f_data));
    double a_reward = get<1>(a_data) / (get<2>(a_data) + !get<2>(a_data));
    double diff = abs(a_reward - f_reward);

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << diff;

    if(a_reward >= f_reward){
        return " A " + oss.str() + " ";
    }
    return " F " + oss.str() + " ";

}
