//
// Created by neria on 12/01/2024.
//

#include "MWU_Agent.h"

MWU_Agent::MWU_Agent(string& name, double beta, unsigned int T, double init_p){
    name_ = name;
    for (int situation = 0; situation <= BB_CO_DE_SB; situation++) {
        for (int hand = 0; hand < 169; hand++) {
            State state{static_cast<Situation>(situation), hand};
            TableEntry entry_a{state, AllIn};
            TableEntry entry_f{state, Fold};

            table_[entry_a] = tuple<double, unsigned int, unsigned int, double, double>{0.0, 0, 0, init_p, init_p};
            table_[entry_f] = tuple<double, unsigned int, unsigned int, double, double>{0.0, 0, 0, 1-init_p, 1-init_p};
        }
    }
    beta_ = beta;
    T_ = T;
}

Action MWU_Agent::get_action(State state) const {
    TableEntry entry_a{state, AllIn};
    TableEntry entry_f{state, Fold};

    double p_a = get<3>(table_.at(entry_a));
    double random = global_uniform_dist(globalGen);
    if (random < p_a)
        return AllIn;
    return Fold;
}

void MWU_Agent::update_parameters(State state, Action action, double reward) {
    TableEntry entry_a{state, AllIn};
    TableEntry entry_f{state, Fold};

    unsigned int N = get<2>(table_.at(entry_a));
    N = N + 1;
    get<2>(table_.at(entry_a)) = N;
    get<2>(table_.at(entry_f)) = N;

    if(action == AllIn){
        get<0>(table_.at(entry_a)) += reward;
        get<1>(table_.at(entry_a)) += 1;
    }
    else{
        get<0>(table_.at(entry_f)) += reward;
        get<1>(table_.at(entry_f)) += 1;
    }

    if (N % T_ == 0){
        double reward_a = get<0>(table_.at(entry_a));
        double reward_f = get<0>(table_.at(entry_f));

        unsigned int n_a = get<1>(table_.at(entry_a));
        unsigned int n_f = get<1>(table_.at(entry_f));
        n_a = n_a + int(!n_a);
        n_f = n_f + int(!n_f);

        reward_a /= n_a;
        reward_f /= n_f;

        double p_a = get<3>(table_.at(entry_a));
        double p_f = get<3>(table_.at(entry_f));

        double p_a_new_tag = p_a * pow(beta_, -reward_a);
        double p_f_new_tag = p_f * pow(beta_, -reward_f);

        double p_a_new = p_a_new_tag / (p_a_new_tag + p_f_new_tag);
        double p_f_new = p_f_new_tag / (p_a_new_tag + p_f_new_tag);

        get<3>(table_.at(entry_a)) = p_a_new; // update probabilities
        get<3>(table_.at(entry_f)) = p_f_new;

        get<4>(table_.at(entry_a)) += p_a_new; // update P
        get<4>(table_.at(entry_f)) += p_f_new;

        get<0>(table_.at(entry_a)) = 0.0; // reset reward
        get<0>(table_.at(entry_f)) = 0.0;

        get<1>(table_.at(entry_a)) = 0; // reset n
        get<1>(table_.at(entry_f)) = 0;
    }
}


string MWU_Agent::ToString() const {
    return "MWU Agent, Beta: " + to_string(beta_) + ", T: " + to_string(T_) + "\n" + Agent::ToString();
}

string MWU_Agent::get_entry_str(State state, int col_w) const {
    int precision = col_w - 4;

    TableEntry entry_a{state, AllIn};
    auto a_data = table_.at(entry_a);
    double P = get<4>(a_data);
    double N = get<2>(a_data);
    int rounds = int(N / T_)+1;

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << P / rounds;
    return " " + oss.str() + " ";

}


