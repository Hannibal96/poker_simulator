//
// Created by neria on 28/12/2021.
//

#include "Q_table.h"

Q_table::Q_table(double alpha, double epsilon) {
    alpha_ = alpha;
    epsilon_ = epsilon;
    table_ = map<TableEntry, double >();
    for(int situation = CO; situation <= BB_CO_DE_SB; situation++){
        for(int hand = 0; hand < 169 ; hand ++){
            for(int act = Fold ; act <= AllIn ; act ++ ){
                State state{static_cast<Situation>(situation), hand};
                TableEntry entry{state, static_cast<Action>(act)};
                table_[entry] = 0.0;
            }
        }
    }

}

Action Q_table::get_action(Situation situation, int hand) {
    uniform_real_distribution<> dis(0.0, 1.0);
    double number = dis(generator);

    if(number < epsilon_){
        number = dis(generator);
        if(number >= 0.5)
            return AllIn;
        return Fold;
    }

    State state{situation, hand};

    double all_in_val = table_[TableEntry{state, AllIn}];
    double fold_val = table_[TableEntry{state, Fold}];

    if(all_in_val >= fold_val)
        return AllIn;
    return Fold;
}

void Q_table::update_table(Situation situation, int hand, Action act, double reward) {
    State state{situation, hand};
    TableEntry entry{state, act};

    table_[entry] = table_[entry] + alpha_*(reward-table_[entry]);
}


