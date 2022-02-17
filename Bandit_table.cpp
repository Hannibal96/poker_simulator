//
// Created by neria on 01/01/2022.
//

#include "Bandit_table.h"

int Bandit_table::class_counter = 0;

Bandit_table::Bandit_table() {
    for(int situation = CO; situation <= BB_CO_DE_SB ; situation++){
        for(int hand = 0; hand < 169 ; hand ++){
            for(int act = Fold ; act <= AllIn ; act ++ ){
                State s{static_cast<Situation>(situation), hand};
                TableEntry entry{s, static_cast<Action>(act)};
                table_[entry] = tuple<double, int, int>{0.0, 0, 0} ;
            }
        }
    }

    static std::random_device rd;  // Will be used to obtain a seed for the random number engine
    static std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    static std::uniform_int_distribution<> dis(1, 999999999);

    seed = dis(gen);

    class_counter ++;
    srand(time(nullptr) + class_counter);   // Initialization, should only be called once.
}

void Bandit_table::update_parameters(Situation situation, double start_epsilon, double final_epsilon, double decay,
                                     double tie_break_epsilon, int decay_cycle){
    situation_epsilon_map[situation] = start_epsilon;
    situation_final_epsilon_map[situation] = final_epsilon;
    situation_decay_map[situation] = decay;
    situation_tie_break_map[situation] = tie_break_epsilon;
    situation_decay_cycle_map[situation] = decay_cycle;
    situation_decay_counter[situation] = 0;
}


double Bandit_table::calc_action_value(Situation situation, int hand, Action act, bool ucb){
    State s{situation, hand};
    TableEntry entry{s, act};

    double total_reward = get<0>(table_[entry]);
    int N = get<1>(table_[entry]);
    int n = get<2>(table_[entry]);

    if(ucb)
        return total_reward / (n+!n) + sqrt(2 * log(N+!N) / (n+!n))  ;

    return total_reward / (n+!n);
}


int Bandit_table::CalcHandIdx(Card a, Card b){
    int hand_num;
    if(a.GetSuit() != b.GetSuit()) {
        if (a.GetValue() >= b.GetValue()) {
            hand_num = (a.GetValue() - 2) * 13 + (b.GetValue() - 2);
        } else {
            hand_num = (b.GetValue() - 2) * 13 + (a.GetValue() - 2);
        }
    }
    else{
        if (a.GetValue() >= b.GetValue()) {
            hand_num = (b.GetValue() - 2) * 13 + (a.GetValue() - 2);
        } else {
            hand_num = (a.GetValue() - 2) * 13 + (b.GetValue() - 2);
        }
    }
    return hand_num;
}

Action Bandit_table::get_action(Situation situation, Card a, Card b){

    srand(seed+class_counter+time(nullptr));
    int hand = CalcHandIdx(a, b);
    double r = (double)rand() / RAND_MAX;

    double epsilon, tie_break_epsilon;
    if(situation_epsilon_map.find(situation) == situation_epsilon_map.end()){ // situation was not specified used first value
        epsilon = situation_epsilon_map.begin()->second;
        tie_break_epsilon = situation_tie_break_map.begin()->second;
    } else {
        epsilon = situation_epsilon_map[situation];
        tie_break_epsilon = situation_tie_break_map[situation];
    }

    if(r <= epsilon) {
        r = rand() % 2;
        if(r == 1)
            return AllIn;
        return Fold;
    }

    double avg_all_in = calc_action_value(situation, hand, AllIn, false)  ;
    double avg_fold = calc_action_value(situation, hand, Fold, false) ;

    if(epsilon == 0){
        avg_all_in = calc_action_value(situation, hand, AllIn, true)  ;
        avg_fold = calc_action_value(situation, hand, Fold, true) ;
    }

    if(avg_all_in > avg_fold)
        return AllIn;
    else if (avg_all_in < avg_fold)
        return Fold;

    r = (double)rand() / RAND_MAX;
    if(r <= tie_break_epsilon)
        return AllIn;
    return Fold;

}

//Action Bandit_table::get_action(State state){
//    return get_action(state.first, state.second);
//}

void Bandit_table::update_table(Situation situation, Card a, Card b, Action act, double reward){

    if(situation_epsilon_map.empty())
        return;

    int hand = CalcHandIdx(a, b);
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

    int counter, cycle_decay;
    double epsilon, final_epsilon, epsilon_decay;
    if(situation_decay_cycle_map.find(situation) == situation_decay_cycle_map.end()){
        cycle_decay = situation_decay_cycle_map.begin()->second;
        epsilon = situation_epsilon_map.begin()->second;
        final_epsilon = situation_final_epsilon_map.begin()->second;
        epsilon_decay = situation_decay_map.begin()->second;
        situation_decay_counter.begin()->second ++;
        counter = situation_decay_counter.begin()->second;
    }else {
        cycle_decay = situation_decay_cycle_map[situation];
        epsilon = situation_epsilon_map[situation];
        final_epsilon = situation_final_epsilon_map[situation];
        epsilon_decay = situation_decay_map[situation];
        situation_decay_counter[situation] ++;
        counter = situation_decay_counter[situation];
    }



    if(counter % cycle_decay == 0 && epsilon > final_epsilon){
        epsilon *= epsilon_decay;
        if(epsilon < final_epsilon)
            epsilon = final_epsilon;

        if(situation_decay_cycle_map.find(situation) == situation_decay_cycle_map.end()){
            situation_epsilon_map.begin()->second = epsilon;
        }else{
            situation_epsilon_map[situation] = epsilon;
        }
    }


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
        string epsilon_str;
        if(situation_epsilon_map.find(static_cast<Situation>(situation)) == situation_epsilon_map.end()){
            //epsilon_str = "~";
            bandit_string += "   "+converter[situation] + ": ~  | ";

        } else{
            //epsilon_str = "\e[1m" + to_string(situation_epsilon_map[static_cast<Situation>(situation)]) + "\e[0m";
            bandit_string += "\e[1m   "+converter[situation] + ": " + to_string(situation_epsilon_map[static_cast<Situation>(situation)]) + "  | \e[0m";
        }

        //bandit_string += "   "+converter[situation] + ": " + epsilon_str + "  | ";
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
                    double diff = calc_action_value(static_cast<Situation>(situation), hand, AllIn, false) -
                                  calc_action_value(static_cast<Situation>(situation), hand, Fold, false);

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



