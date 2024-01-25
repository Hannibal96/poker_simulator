//
// Created by Neria on 28/06/2020.
//

#ifndef POKER_SIMULATOR_POKERTABLE_H
#define POKER_SIMULATOR_POKERTABLE_H

#include <vector>
#include "../agents_utils/PokerPlayer.h"
#include "../deck_utils/Deck.h"
#include "Enums.h"

using namespace std;

class PokerTable {

private:
    vector<PokerPlayer> players;
    Deck deck;
    vector<Card> community_cards;
    int curr_co_idx;
    double curr_pot;
    double big_blind_, small_blind_, all_in_, jack_pot_;
    long unsigned int hands_counter;
    int repeats_;
    History curr_history;

    map<HandRank, long double> hands_stats;
    map<History, long unsigned int> scenarios_stats;
    map<Position, double> jackpots_occur;
    bool update_positions_;

    void EndRound();
    void StartRound();
    void take_blinds();
    void deal_cards();
    void deal_community_cards();
    void players_action();
    void total_money_sanity_check();
    vector<int> GetWinners();
    void UpdateHistory(Position position, Action action);
    void UpdateHandsStats(uint32_t hand_hash, unsigned int repeats);

public:
    PokerTable(vector<PokerPlayer> players,
               double big_blind, double small_blind, double all_in, double jackpot,
               bool update_positions, int repeats);
    ~PokerTable() = default;
    void Round();
    string GetStatsString(int iteration);
    string ToString() ;
};

std::ostream& operator<<(std::ostream& os, PokerTable& table);

#endif //POKER_SIMULATOR_POKERTABLE_H
