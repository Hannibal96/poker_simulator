//
// Created by Neria on 28/06/2020.
//

#include "PokerTable.h"
#include <bits/stdc++.h>
#include <utility>


PokerTable::PokerTable(vector<PokerPlayer> players_vec,
                       double big_blind, double small_blind, double all_in, double jackpot,
                       bool update_positions, int repeats) {

    players = vector<PokerPlayer>(std::move(players_vec));

    big_blind_ = big_blind;
    small_blind_ = small_blind;
    all_in_ = all_in;
    jack_pot_ = jackpot;

    repeats_ = repeats;

    curr_pot = 0;
    curr_history = BigBlind_In;

    curr_co_idx = 0;
    hands_counter = 0;

    update_positions_ = update_positions;

    hands_stats = map<HandRank, long double>();
    for (int rank = HighCArd; rank != StraightFlush + 1; rank++) {
        auto hand_rank = static_cast<HandRank >(rank);
        hands_stats[hand_rank] = 0;
    }

    for (int scenario = BigBlind_In; scenario != CutOff_Dealer_SmallBlind_BigBlind_In + 1; scenario++) {
        auto curr_scenario = static_cast<History >(scenario);
        scenarios_stats[curr_scenario] = 0;
    }

    for(int pos = BigBlind; pos < CutOff + 1; pos++){
        auto curr_pos = static_cast<Position >(pos);
        jackpots_occur[curr_pos] = 0;
    }

}


void PokerTable::UpdateHistory(Position position, Action action) {
    assert(action != NotAct);
    if(action == Fold)
        return;

    if(curr_history == SmallBlind_In){
        if(position == BigBlind) {
            curr_history = SmallBlind_BigBlind_In;
            return;
        }
    }

    else if(curr_history == Dealer_In){
        if(position == SmallBlind) {
            curr_history = Dealer_SmallBlind_In;
            return;
        }
        else if(position == BigBlind) {
            curr_history = Dealer_BigBlind_In;
            return;
        }
    }

    else if(curr_history == Dealer_SmallBlind_In){
        if(position == BigBlind) {
            curr_history = Dealer_SmallBlind_BigBlind_In;
            return;
        }
    }

    else if(curr_history == CutOff_In){
        if(position == Dealer) {
            curr_history = CutOff_Dealer_In;
            return;
        }
        else if(position == SmallBlind) {
            curr_history = CutOff_SmallBlind_In;
            return;
        }
        else if(position == BigBlind) {
            curr_history = CutOff_BigBlind_In;
            return;
        }

    }

    else if(curr_history == CutOff_Dealer_In){
        if(position == SmallBlind)        {
            curr_history = CutOff_Dealer_SmallBlind_In;
            return;
        }
        else if(position == BigBlind)        {
            curr_history = CutOff_Dealer_BigBlind_In;
            return;
        }
    }

    else if(curr_history == CutOff_SmallBlind_In){
        if(position == BigBlind){
            curr_history = CutOff_SmallBlind_BigBlind_In;
            return;
        }
    }

    else if(curr_history == CutOff_Dealer_SmallBlind_In){
        if(position == BigBlind)        {
            curr_history = CutOff_Dealer_SmallBlind_BigBlind_In;
            return;
        }
    }

    else if(curr_history == BigBlind_In){
        if(position == CutOff){
            curr_history = CutOff_In;
            return;
        }
        else if (position == Dealer) {
            curr_history = Dealer_In;
            return;
        }
        else if (position == SmallBlind)        {
            curr_history = SmallBlind_In;
            return;
        }
        else if (position == BigBlind) {
            curr_history = BigBlind_In;
            return;
        }
    }

    cout << curr_history << endl;
    assert(false);
}

void PokerTable::EndRound() {
    for(int i=0;i<TABLE_SIZE;i++){
        players[i].UpdateTable();
    }
}

void PokerTable::StartRound() {
    curr_pot = 0;
    if(update_positions_)
        curr_co_idx = (curr_co_idx + 1) % TABLE_SIZE;
    for(int i=0;i<TABLE_SIZE;i++){
        if(update_positions_)
            players[i].UpdatePosition();
        players[i].ResetReward();
        players[i].MockHand();
        players[i].UnSetAction();
    }
    curr_history = BigBlind_In;
    community_cards.clear();

    deck = Deck();
    deck.Shuffle();
}

void PokerTable::take_blinds(){
    for(int i=0 ; i<TABLE_SIZE ; i++){    // post blinds
        if(players[i].GetPosition() == SmallBlind){
            players[i].UpdateMoney(-small_blind_);
            curr_pot += small_blind_;
        }
        else if (players[i].GetPosition() == BigBlind){
            players[i].UpdateMoney(-big_blind_);
            curr_pot += big_blind_;
        }
    }
}

void PokerTable::players_action(){
    for(int i=0 ; i<TABLE_SIZE ; i++){      // playing

        Action action = players[(curr_co_idx+i)%TABLE_SIZE].GetAction(curr_history); // starts plays from CO
        UpdateHistory(players[(curr_co_idx+i)%TABLE_SIZE].GetPosition(), action);

        if(action == AllIn) {
            if (players[(curr_co_idx + i) % TABLE_SIZE].GetPosition() == CutOff
                || players[(curr_co_idx + i) % TABLE_SIZE].GetPosition() == Dealer) {
                players[(curr_co_idx + i) % TABLE_SIZE].UpdateMoney(-all_in_);
                curr_pot += all_in_;
            } else if (players[(curr_co_idx + i) % TABLE_SIZE].GetPosition() == SmallBlind) {
                players[(curr_co_idx + i) % TABLE_SIZE].UpdateMoney(-all_in_ + small_blind_);
                curr_pot += (+all_in_ - small_blind_);
            } else if (players[(curr_co_idx + i) % TABLE_SIZE].GetPosition() == BigBlind) {
                players[(curr_co_idx + i) % TABLE_SIZE].UpdateMoney(-all_in_ + big_blind_);
                curr_pot += (+all_in_ - big_blind_);
            }
        }
    }
}

vector<int> PokerTable::GetWinners() {
    bool flag_01_eq, flag_23_eq, flag_01_ge, flag_23_ge,
            flag_01_lo, flag_23_lo, flag_final_eq, flag_final_ge, flag_final_lo;
    bool show_down = curr_pot >= 2 * all_in_ ;
    flag_01_eq = (players[0] == players[1]);
    flag_01_ge = (players[0] > players[1]);
    flag_01_lo = !(flag_01_eq || flag_01_ge);
    flag_23_eq = (players[2] == players[3]);
    flag_23_ge = (players[2] > players[3]);
    flag_23_lo = !(flag_23_eq || flag_23_ge);

    flag_final_eq = players[(int)(flag_01_lo)] == players[2+(int)(flag_23_lo)];
    flag_final_ge = players[(int)(flag_01_lo)] > players[2+(int)(flag_23_lo)];
    flag_final_lo = !(flag_final_ge || flag_final_eq);

    vector<int> winning_idx = vector<int>();
    //1000
    if(flag_01_ge and flag_final_ge){
        if (players[0].IsJAckPot(community_cards) and show_down) {
            players[0].UpdateMoney(jack_pot_ / repeats_);
            jackpots_occur[players[0].GetPosition()] += 1.0 / repeats_;
        }
        winning_idx.push_back(0);
    }
    //0100
    else if(flag_01_lo and flag_final_ge){
        if (players[1].IsJAckPot(community_cards) and show_down) {
            players[1].UpdateMoney(jack_pot_ / repeats_);
            jackpots_occur[players[1].GetPosition()] += 1.0 / repeats_;
        }
        winning_idx.push_back(1);
    }
    //0010
    else if(flag_23_ge and flag_final_lo){
        if (players[2].IsJAckPot(community_cards) and show_down) {
            players[2].UpdateMoney(jack_pot_ / repeats_);
            jackpots_occur[players[2].GetPosition()] += 1.0 / repeats_;
        }
        winning_idx.push_back(2);
    }
    //0001
    else if(flag_23_lo and flag_final_lo){
        if (players[3].IsJAckPot(community_cards) and show_down) {
            players[3].UpdateMoney(jack_pot_ / repeats_);
            jackpots_occur[players[3].GetPosition()] += 1.0 / repeats_;
        }
        winning_idx.push_back(3);
    }
    //1100
    else if(flag_01_eq and flag_final_ge){
        winning_idx.push_back(0);
        winning_idx.push_back(1);
    }
    //1010
    else if(flag_01_ge and flag_23_ge and flag_final_eq){
        winning_idx.push_back(0);
        winning_idx.push_back(2);
    }
    //1001
    else if(flag_01_ge and flag_23_lo and flag_final_eq){
        winning_idx.push_back(0);
        winning_idx.push_back(3);
    }
    //0110
    else if(flag_01_lo and flag_23_ge and flag_final_eq){
        winning_idx.push_back(1);
        winning_idx.push_back(2);
    }
    //0101
    else if(flag_01_lo and flag_23_lo and flag_final_eq){
        winning_idx.push_back(1);
        winning_idx.push_back(3);
    }
    //0011
    else if(flag_23_eq and flag_final_lo){
        winning_idx.push_back(2);
        winning_idx.push_back(3);
    }
    //1110
    else if(flag_final_eq and flag_01_eq and flag_23_ge){
        winning_idx.push_back(0);
        winning_idx.push_back(1);
        winning_idx.push_back(2);
    }
    //1101
    else if(flag_final_eq and flag_01_eq and flag_23_lo){
        winning_idx.push_back(0);
        winning_idx.push_back(1);
        winning_idx.push_back(3);
    }
    //1011
    else if(flag_final_eq and flag_01_ge and flag_23_eq){
        winning_idx.push_back(0);
        winning_idx.push_back(2);
        winning_idx.push_back(3);
    }
    //0111
    else if(flag_final_eq and flag_01_lo and flag_23_eq){
        winning_idx.push_back(1);
        winning_idx.push_back(2);
        winning_idx.push_back(3);
    }
    //1111
    else if(flag_23_eq and flag_final_eq and flag_01_eq){
        winning_idx.push_back(0);
        winning_idx.push_back(1);
        winning_idx.push_back(2);
        winning_idx.push_back(3);
    }
    else
        assert(false);
    return winning_idx;
}

void PokerTable::deal_cards() {
    for(int i=0 ; i<TABLE_SIZE*2 ; i++){    // deal cards
        players[(curr_co_idx+2+i)%TABLE_SIZE].AddHoldingCards(deck.DealCard()); // starts deal from SB
    }
}

void PokerTable::deal_community_cards() {
    community_cards.clear();
    deck.DealCard();
    community_cards.push_back(deck.DealCard());
    community_cards.push_back(deck.DealCard());
    community_cards.push_back(deck.DealCard());
    deck.DealCard();
    community_cards.push_back(deck.DealCard());
    deck.DealCard();
    community_cards.push_back(deck.DealCard());
}


void PokerTable::RunRounds(int rounds) {
    for(int i=0 ; i<rounds ; i++){
        Round();
    }
}


void PokerTable::Round() {

    StartRound(); // update positions, clear cards and rewards, new deck and shuffle
    take_blinds();
    deal_cards();
    players_action();

    // FIXME: check if there is a showdown!
    bool show_down = curr_pot >= 2 * all_in_ ;
    for(int r=0 ; r < repeats_ * show_down + not show_down; r++) {        // Deal Comm Cards + Evaluate
        // r={repeates, 1} deal at least once for statistics purposes
        if(r != 0 && r%5 ==0){
            deck = Deck();
            for(int i=0 ; i<TABLE_SIZE ; i++){
                deck.DisCard(players[i].GetHoldingCard1());
                deck.DisCard(players[i].GetHoldingCard2());
            }
            deck.Shuffle();
        }
        deal_community_cards();

        for (int i = 0; i < TABLE_SIZE; i++) {      // evaluate hands
            players[i].EvaluateHandHash(community_cards); // TODO: Maybe skip folders since it's not necessary
        }

        vector<int> winning_idx = GetWinners();
        unsigned int winning_counter=winning_idx.size();

        double wins_money = (double) (curr_pot) / ((repeats_ * show_down + not show_down) * winning_counter);
        for(auto idx: winning_idx){
            players[idx].UpdateMoney(wins_money);
        }

        for(auto const & x :players){
            UpdateHandsStats(x.GetPlayerBestHashHand(), repeats_ * show_down + not show_down);
        }
        /* FIXME: IDEA - mabye sum all the money per player and only add it once*/
    }

    scenarios_stats[curr_history] ++;

    hands_counter++;
    total_money_sanity_check();
    EndRound(); // update players tables
}

void PokerTable::total_money_sanity_check() {
    long double total_money = 0;
    for(auto const & x :players){
        total_money += x.GetMoney();
    }
    double total_jp = 0;
    for(auto pos : {Position::CutOff, Position::Dealer, Position::SmallBlind, Position::BigBlind}){
        total_jp += jackpots_occur[pos];
    }
    assert(abs(total_money - total_jp * jack_pot_) <= 0.1 && "-ASSERT- not zero sum of money, ");
}

void PokerTable::UpdateHandsStats(uint32_t hand_hash, unsigned int repeats){
    if(hand_hash < 1296)
        hands_stats[HighCArd] += 1.0 / repeats;
    else if(hand_hash < 4141)
        hands_stats[Pair] += 1.0 / repeats;
    else if(hand_hash < 5004)
        hands_stats[TwoPairs] += 1.0 / repeats;
    else if(hand_hash < 5854)
        hands_stats[Trips] += 1.0 / repeats;
    else if(hand_hash < 5864)
        hands_stats[Straight] += 1.0 / repeats;
    else if(hand_hash < 7141)
        hands_stats[Flush] += 1.0 / repeats;
    else if(hand_hash < 7297)
        hands_stats[FullHouse] += 1.0 / repeats;
    else if(hand_hash < 7453)
        hands_stats[Quads] += 1.0 / repeats;
    else if(hand_hash <= 7462)
        hands_stats[StraightFlush] += 1.0 / repeats;
    else
        assert(false);
}

uint64_t PokerTable::GetHandsCounter() const{
    return hands_counter;
}


string PokerTable::GetStatsString(int iteration) {
    string stats_string;
    for(auto x: hands_stats){
        stats_string += ranks_names.at(x.first) + ": " + to_string(100*(double)(x.second)/(TABLE_SIZE * iteration)) + "\n";
    }
    stats_string += "\n";
    double percentage_sum = 0;
    for(auto x: scenarios_stats){
        stats_string += scenarios_names.at(x.first) + ": " + to_string(100*(double)(x.second)/(iteration)) + "\n";
        percentage_sum += 100*(double)(x.second)/(iteration);
    }

    stats_string += "\nJackpots:\n";
    for(auto pos : {Position::CutOff, Position::Dealer, Position::SmallBlind, Position::BigBlind}){
        stats_string += positions_names.at(pos) + ":" + to_string(100.0 * jackpots_occur[pos] / iteration) + "\n";
    }

    stats_string += "\n";
    if(abs(100-percentage_sum) > 0.01){
        cout << "==========================" << endl;
        cout << percentage_sum << endl;
        cout << "==========================" << endl;
        exit(8);
    }
    assert(abs(100-percentage_sum) < 0.01);

    return stats_string;
}

string PokerTable::ToString() {        //TODO: change printing format to something more nice
    string table_string = "=====================================================================\n";
    table_string += "========= Poker Table Summary: #Hand: "+to_string(hands_counter)+"\n\n| ";

    for(auto x: community_cards){
        table_string += x.ToString()+" | ";
    }
    table_string += "\n";

    for(int i=0; i<TABLE_SIZE; i++){
        table_string += players[i].ToString();
    }
    table_string += "\n=====================================================================\n";
    return table_string+"\n";

}

std::ostream& operator<<(std::ostream& os,  PokerTable& table){
    os << table.ToString();
    return os;
}
