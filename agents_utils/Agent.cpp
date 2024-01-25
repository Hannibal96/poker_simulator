//
// Created by neria on 24/01/2024.
//
#include "Agent.h"


std::ostream& operator<<(std::ostream& os, const Agent& agent){
    os << agent.ToString();
    return os;
}

string Agent::ToString() const{
    int col_width = 13, len, pad, tail;
    string bandit_string = "    ";
    for (int situation = 0; situation <= BB_CO_DE_SB; situation++) {
        string situation_name = situation_names.at(static_cast<Situation>(situation));
        len = int(situation_name.length());
        pad = (col_width - len) / 2;
        tail = (col_width - len) % 2;
        bandit_string += "|" + string(pad, ' ') + situation_name + string(pad + tail, ' ');
    }
    bandit_string += "|\n";
    for (int hand = 0; hand < 169; hand++) {
        bandit_string += idx_to_hand_string(hand) + " |";
        for (int situation = 0; situation <= BB_CO_DE_SB; situation++) {
            State state{static_cast<Situation>(situation), hand};
            bandit_string += get_entry_str(state, col_width) + "|";
        }
        bandit_string += "\n";
    }

    return bandit_string;

}

