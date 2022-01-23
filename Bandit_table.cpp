//
// Created by neria on 01/01/2022.
//

#include "Bandit_table.h"

Bandit_table::Bandit_table(double epsilon) {
    epsilon_ = epsilon;
    for(int situation = CO; situation <= BB_CO_DE_SB ; situation++){
        for(int hand = 0; hand < 169 ; hand ++){
            for(int act = Fold ; act <= AllIn ; act ++ ){
                State s{static_cast<Situation>(situation), hand};
                TableEntry entry{s, static_cast<Action>(act)};
                table_[entry] = tuple<double, int, int>{0.0, 0, 0} ;
            }
        }
    }
}

double Bandit_table::calc_ucb(Situation situation, int hand, Action act){
    State s{situation, hand};
    TableEntry entry{s, act};

    double total_reward = get<0>(table_[entry]);
    int N = get<1>(table_[entry]);
    int n = get<2>(table_[entry]);

    return total_reward / (n+!n) + sqrt(2 * log(N+!N) / (n+!n))  ;
}


Action Bandit_table::get_action(Situation situation, int hand){

    double UCB_all_in = calc_ucb(situation, hand, AllIn)  ;
    double UCB_fold = calc_ucb(situation, hand, Fold) ;

    if(UCB_all_in == UCB_fold){
        std::random_device rd;  // Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
        std::uniform_real_distribution<> dis(0.0, 1.0);

        double r = dis(gen);
        if(r >= epsilon_)
            return AllIn;
        return Fold;
    }

    if(UCB_all_in >= UCB_fold )
        return AllIn;
    return Fold;
}

Action Bandit_table::get_action(State state){
    return get_action(state.first, state.second);
}

void Bandit_table::update_table(Situation situation, int hand, Action act, double reward){
    State s{situation, hand};
    TableEntry entry, comp_entry;

    if(act == AllIn){
        entry = TableEntry {s, AllIn};
        comp_entry  = TableEntry {s, Fold};
    }
    else if (act == Fold){
        entry = TableEntry {s, Fold};
        comp_entry = TableEntry {s, AllIn};
    }

    get<0>(table_[entry]) += reward;

    get<1>(table_[entry]) ++;
    get<1>(table_[comp_entry]) ++;

    get<2>(table_[entry]) ++;
}


string num2string(int num){
    int val_1 = num / 13;
    int val_2 = num % 13;
    string card_1, card_2;
    map<int, string> converter = {
            {0, "2"},
            {1, "3"},
            {2, "4"},
            {3, "5"},
            {4, "6"},
            {5, "7"},
            {6, "8"},
            {7, "9"},
            {8, "T"},
            {9, "J"},
            {10, "Q"},
            {11, "K"},
            {12, "A"}    };
    card_1 = converter[val_1];
    card_2 = converter[val_2];
    if(val_1 < val_2)
        return card_2+card_1+'s';
    else
        return card_1+card_2+'o';
}


string Bandit_table::ToString() {

    map<int, string> converter = {
            {0, "CO"},
            {1, "DE"},
            {2, "DE_CO"},
            {3, "SB"},
            {4, "SB_CO"},
            {5, "SB_DE"},
            {6, "SB_CO_DE"},
            {7, "BB_CO"},
            {8, "BB_DE"},
            {9, "BB_SB"},
            {10, "BB_CO_DE"},
            {11, "BB_CO_SB"},
            {12, "BB_DE_SB"},
            {13, "BB_CO_DE_SB"}            };

    string bandit_string = "\n=====================================================================\n";
    bandit_string += "========== Bandit Table: \n\t|";

    for(int situation=CO; situation <= BB_CO_DE_SB; situation++){
        bandit_string += "\t  "+converter[situation] + " | \t ";
    }

    for(int card_a = 0 ; card_a < 13 ; card_a++){
        for(int card_b = 0 ; card_b <= card_a ; card_b++){


        }
    }

    for(int val_1=0; val_1<13; val_1++){
        for(int val_2=0; val_2<=val_1; val_2++){
            for(int suit=0; suit<=1; suit++){
                string temp_string;
                bool change = false;
                int hand;

                if(suit == 1 && val_1 == val_2) continue;
                if(suit == 0)                hand = val_1 * 13 + val_2;
                if(suit == 1)                hand = val_2 * 13 + val_1;

                temp_string += "\n"+ num2string(hand) + "\t|";

                for(int situation=CO; situation <= BB_CO_DE_SB; situation++){
                    double diff = calc_ucb(static_cast<Situation>(situation), hand, AllIn) -
                                  calc_ucb(static_cast<Situation>(situation), hand, Fold);
                    change = change or ((bool) diff);

                    if(diff != 0){
                        temp_string += "\e[1m";
                    }

                    if(diff >= 0) {
                        temp_string += "A " + to_string(diff) + " |\t";
                    }
                    else{
                        temp_string += "F " + to_string(-diff) + " |\t";
                    }
                    if(diff != 0){
                        temp_string += "\e[0m";
                    }
                }
                if(change)
                    bandit_string += temp_string;

            }
        }
    }

    return bandit_string + "\n";
}

std::ostream& operator<<(std::ostream& os, Bandit_table& table){
    os << table.ToString();
    return os;
}



