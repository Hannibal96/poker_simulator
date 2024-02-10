//
// Created by Neria on 28/06/2020.
//

#ifndef POKER_SIMULATOR_POKERTABLE_H
#define POKER_SIMULATOR_POKERTABLE_H

#include <vector>
#include "../agents_utils/PokerPlayer.h"
#include "../deck_utils/Deck.h"
#include "Enums.h"
#include <mutex>

using namespace std;

class PokerTable {

private:
    vector<PokerPlayer> players;
    Deck deck;
    vector<Card> community_cards;
    int curr_co_idx;
    double curr_pot;
    double big_blind_, small_blind_, all_in_, jack_pot_;
    uint64_t hands_counter;
    int repeats_;
    History curr_history;
    bool update_positions_;

    map<HandRank, long double> private_hands_stats;
    map<History, long unsigned int> private_scenarios_stats;
    map<Position, double> private_jackpots_occur;

    static map<HandRank, long double> hands_stats;
    static map<History, long unsigned int> scenarios_stats;
    static map<Position, double> jackpots_occur;
    static std::vector<PokerTable*> pt_instances;

    void Round();
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
    ~PokerTable();
    void RunRounds(int rounds);
    string ToString() ;
    static string GetStatsString(int iteration);
    uint64_t GetHandsCounter() const;
    static void initializeStatistics();
    static mutex print_mutex;
    static mutex write_mutex;
    static uint64_t total_hands_counter;
    static uint64_t print_count;
};

std::ostream& operator<<(std::ostream& os, PokerTable& table);

#endif //POKER_SIMULATOR_POKERTABLE_H
