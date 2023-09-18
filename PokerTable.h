//
// Created by Neria on 28/06/2020.
//

#ifndef POKER_SIMULATOR_POKERTABLE_H
#define POKER_SIMULATOR_POKERTABLE_H

#include <vector>
#include "PokerPlayer.h"
#include "Deck.h"
#include "SlimDeck.h"
#include "Enums.h"

using namespace std;

class PokerTable {

private:
    vector<PokerPlayer> players;
    vector<Card> community_cards;
    int curr_co_idx;
    double curr_pot;
    double big_blind_, small_blind_, all_in_, jack_pot_;
    int table_id_;
    long unsigned int hands_counter;
    int repeats_;
    History curr_history;

    map<HandRank, long unsigned int> hands_stats;
    map<HandRank, string> ranks_names;

    map<History, long unsigned int> scenarios_stats;
    map<History, string> scenarios_names;
    bool update_positions_;
    void EndRound();
    void StartRound();
    void UpdateHistory(Position position, Action action);

public:
    PokerTable(PokerPlayer const & player_a, PokerPlayer const & player_b,
               PokerPlayer const & player_c, PokerPlayer const & player_d,
               double big_blind, double small_blind, double all_in, double jackpot,
               int table_id, bool update_positions, int repeats);
    ~PokerTable() = default;
    void Round();
    string GetStatsSring(int iteration);
    string ToString() ;

};

static bool SortByHand(PokerPlayer p1, PokerPlayer p2);

static bool SortByID(PokerPlayer p1, PokerPlayer p2);

static void UpdateHandsStats(map<HandRank, long unsigned int > & ranks_stats,uint32_t hand);


std::ostream& operator<<(std::ostream& os, PokerTable& table);

#endif //POKER_SIMULATOR_POKERTABLE_H
