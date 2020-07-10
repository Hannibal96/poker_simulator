//
// Created by Neria on 28/06/2020.
//

#ifndef POKER_SIMULATOR_POKERTABLE_H
#define POKER_SIMULATOR_POKERTABLE_H

#include <vector>
#include "PokerPlayer.h"
#include "Deck.h"
#include "Enums.h"

using namespace std;

class PokerTable {

private:
    vector<PokerPlayer> players;
    vector<Card> community_cards;
    int curr_co_idx;
    double curr_pot;
    double big_blind_, small_blind_, all_in_;
    int table_id_;
    int hands_counter, show_down_counter;
    PreviousAction previous_action;
    void EndRound();

public:
    PokerTable(PokerPlayer plater_a, PokerPlayer plater_b, PokerPlayer plater_c, PokerPlayer plater_d,
            double big_blind, double small_blind, double all_in, int table_id);
    ~PokerTable() = default;
    void Round();

    string ToString() const;

};
static bool SortByID(PokerPlayer p1, PokerPlayer p2);

std::ostream& operator<<(std::ostream& os, const PokerTable& table);

#endif //POKER_SIMULATOR_POKERTABLE_H
