//
// Created by Neria on 28/06/2020.
//

#include "PokerTable.h"
#include <bits/stdc++.h>


PokerTable::PokerTable(PokerPlayer const & player_a , PokerPlayer const & player_b,
                       PokerPlayer const & player_c, PokerPlayer const & player_d,
                       double big_blind, double small_blind, double all_in, double jackpot,
                       int table_id, bool update_positions, int repeats) {

    players.push_back(player_a);
    players.push_back(player_b);
    players.push_back(player_c);
    players.push_back(player_d);

    big_blind_ = big_blind;
    small_blind_ = small_blind;
    all_in_ = all_in;
    jack_pot_ = jackpot;

    table_id_ = table_id;

    repeats_ = repeats;

    curr_pot = 0;
    curr_history = BigBlind_In;

    curr_co_idx = 0;
    hands_counter = 0;

    update_positions_ = update_positions;

    hands_stats = map<HandRank, long unsigned int>();
    for (int rank = HighCArd; rank != StraightFlush + 1; rank++) {
        auto hand_rank = static_cast<HandRank >(rank);
        hands_stats[hand_rank] = 0;
    }

    for (int scenario = BigBlind_In; scenario != CutOff_Dealer_SmallBlind_BigBlind_In + 1; scenario++) {
        auto curr_scenario = static_cast<History >(scenario);
        scenarios_stats[curr_scenario] = 0;
    }

    ranks_names = { { UnSet, "UnSet" },
                    { HighCArd, "HighCard" },
                    { Pair, "Pair" },
                    { TwoPairs, "TwoPairs" },
                    { Trips, "Trips" },
                    { Straight, "Straight" },
                    { Flush, "Flush" },
                    { FullHouse, "FullHouse" },
                    { Quads, "Quads" },
                    { StraightFlush, "StraightFlush" }, };

    jackpots_occur[CutOff] = 0;
    jackpots_occur[Dealer] = 0;
    jackpots_occur[SmallBlind] = 0;
    jackpots_occur[BigBlind] = 0;

    scenarios_names = {{ BigBlind_In, "Empty-BigBlind" },
                       { CutOff_In, "OneRaise-CutOff" },
                       { Dealer_In, "OneRaise-Dealer" },
                       { SmallBlind_In, "OneRaise-SmallBlind" },
                       { CutOff_Dealer_In, "TwoRaises-CutOff, Dealer" },
                       { CutOff_SmallBlind_In, "TwoRaises-CutOff, SmallBlind" },
                       { CutOff_BigBlind_In, "TwoRaises-CutOff, BigBlind" },
                       { Dealer_SmallBlind_In, "TwoRaises-Dealer, SmallBlind" },
                       { Dealer_BigBlind_In, "TwoRaises-Dealer, BigBlind" },
                       { SmallBlind_BigBlind_In, "TwoRaises-SmallBlind, BigBlind" },
                       { CutOff_Dealer_SmallBlind_In, "ThreeRaises-CutOff, Dealer, SmallBlind" },
                       { CutOff_Dealer_BigBlind_In, "ThreeRaises-CutOff, Dealer, BigBlind" },
                       { CutOff_SmallBlind_BigBlind_In, "ThreeRaises-CutOff, SmallBlind, BigBlind" },
                       { Dealer_SmallBlind_BigBlind_In, "ThreeRaises-Dealer, SmallBlind, BigBlind" },
                       { CutOff_Dealer_SmallBlind_BigBlind_In, "FourRaises-CutOff, Dealer, SmallBlind, BigBlind" },};

    positions_names = {{Position::BigBlind, "BigBlind"},
                       {Position::SmallBlind, "SmallBlind"},
                       {Position::Dealer, "Dealer"},
                       {Position::CutOff, "CutOff"},
                       };

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

void PokerTable::Round() {

    StartRound();

    Deck deck = Deck();
    deck.Shuffle();

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

    for(int i=0 ; i<TABLE_SIZE*2 ; i++){    // deal cards
        players[(curr_co_idx+2+i)%TABLE_SIZE].AddHoldingCards(deck.DealCard()); // starts deal from SB
    }

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

    // FIXME:
    //  BUG: check if there is a showdown!

    bool show_down = curr_pot >= 2 * all_in_ ;

    for(int r=0 ; r < repeats_ * show_down + not show_down; r++) {        // Deal Comm Cards + Evaluate
        if(r != 0 && r%5 ==0){
            deck = Deck();
            for(int i=0 ; i<TABLE_SIZE ; i++){
                deck.DisCard(players[i].GetHoldingCard1());
                deck.DisCard(players[i].GetHoldingCard2());
            }
            deck.Shuffle();
        }

        community_cards.clear();

        // deal community cards
        deck.DealCard();
        community_cards.push_back(deck.DealCard());
        community_cards.push_back(deck.DealCard());
        community_cards.push_back(deck.DealCard());
        deck.DealCard();
        community_cards.push_back(deck.DealCard());
        deck.DealCard();
        community_cards.push_back(deck.DealCard());

        for (int i = 0; i < TABLE_SIZE; i++) {      // evaluate hands
            players[i].EvaluateHandHash(community_cards);
        }

        bool flag_01_eq, flag_23_eq, flag_01_ge, flag_23_ge, flag_01_lo, flag_23_lo, flag_final_eq, flag_final_ge, flag_final_lo;
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
            if(players[0].GetPlayerBestHashHand() >= 7453) {
                if (players[0].IsJAckPot(community_cards) and show_down) {
                    players[0].UpdateMoney(jack_pot_ / repeats_);
                    jackpots_occur[players[0].GetPosition()] += 1.0 / repeats_;
                }
            }
            winning_idx.push_back(0);
        }
        //0100
        else if(flag_01_lo and flag_final_ge){
             if(players[1].GetPlayerBestHashHand() >= 7453) {
                 if (players[1].IsJAckPot(community_cards) and show_down) {
                     players[1].UpdateMoney(jack_pot_ / repeats_);
                     jackpots_occur[players[1].GetPosition()] += 1.0 / repeats_;
                 }
             }
            winning_idx.push_back(1);
        }
        //0010
        else if(flag_23_ge and flag_final_lo){
             if(players[2].GetPlayerBestHashHand() >= 7453) {
                 if (players[2].IsJAckPot(community_cards) and show_down) {
                     players[2].UpdateMoney(jack_pot_ / repeats_);
                     jackpots_occur[players[2].GetPosition()] += 1.0 / repeats_;
                 }
             }
            winning_idx.push_back(2);
        }
        //0001
        else if(flag_23_lo and flag_final_lo){
             if(players[3].GetPlayerBestHashHand() >= 7453) {
                 if (players[3].IsJAckPot(community_cards) and show_down) {
                     players[3].UpdateMoney(jack_pot_ / repeats_);
                     jackpots_occur[players[3].GetPosition()] += 1.0 / repeats_;
                 }
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

        unsigned int winning_counter=winning_idx.size();

        double wins_money = (double) (curr_pot) / ((repeats_ * show_down + not show_down) * winning_counter);
        for(auto idx: winning_idx){
            players[idx].UpdateMoney(wins_money);
        }
        /* FIXME: IDEA - mabye sum all the money per player and only add it once*/
    }

    scenarios_stats[curr_history] ++;

    hands_counter++;
    double total_money = 0;
    for(auto const & x :players){
        total_money += x.GetMoney();
        UpdateHandsStats(hands_stats, x.GetPlayerBestHashHand());
    }

    if (jack_pot_ == 0){
        assert(abs(total_money) <= 0.1 && "-ASSERT- none zero sum of money, ");
    }

    EndRound();
}

static void UpdateHandsStats(map<HandRank, long unsigned int > & ranks_stats, uint32_t hand_hash ){
    if(hand_hash < 1296)
        ranks_stats[HighCArd] ++;
    else if(hand_hash < 4141)
        ranks_stats[Pair] ++;
    else if(hand_hash < 5004)
        ranks_stats[TwoPairs] ++;
    else if(hand_hash < 5854)
        ranks_stats[Trips] ++;
    else if(hand_hash < 5864)
        ranks_stats[Straight] ++;
    else if(hand_hash < 7141)
        ranks_stats[Flush] ++;
    else if(hand_hash < 7297)
        ranks_stats[FullHouse] ++;
    else if(hand_hash < 7453)
        ranks_stats[Quads] ++;
    else if(hand_hash <= 7462)
        ranks_stats[StraightFlush] ++;
    else
        assert(false);
}


void PokerTable::EndRound() {

    for(int i=0;i<TABLE_SIZE;i++){
        players[i].UpdateTable();
        // FIXME BUG! updating by last curr reward
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

}


string PokerTable::GetStatsSring(int iteration) {
    string stats_string;
    for(auto x: hands_stats){
        stats_string += ranks_names[x.first] + ": " + to_string(100*(double)(x.second)/(TABLE_SIZE * iteration)) + "\n";
    }
    stats_string += "\n";
    double percentage_sum = 0;
    for(auto x: scenarios_stats){
        stats_string += scenarios_names[x.first] + ": " + to_string(100*(double)(x.second)/(iteration)) + "\n";
        percentage_sum += 100*(double)(x.second)/(iteration);
    }

    stats_string += "\nJackpots:\n";
    for(auto pos : {Position::CutOff, Position::Dealer, Position::SmallBlind, Position::BigBlind}){
        stats_string += positions_names[pos] + ":" + to_string(100.0 * jackpots_occur[pos] / iteration) + "\n";
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
    table_string += "========= Table: "+to_string(table_id_)+", #Hand: "+to_string(hands_counter)+"\n\n| ";

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
