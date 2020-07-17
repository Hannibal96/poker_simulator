//
// Created by Neria on 28/06/2020.
//

#include "PokerTable.h"
#include <bits/stdc++.h>


PokerTable::PokerTable(PokerPlayer plater_a, PokerPlayer plater_b, PokerPlayer plater_c, PokerPlayer plater_d,
                       double big_blind, double small_blind, double all_in, int table_id) {

    players.push_back(plater_a);
    players.push_back(plater_b);
    players.push_back(plater_c);
    players.push_back(plater_d);

    big_blind_ = big_blind;
    small_blind_ = small_blind;
    all_in_ = all_in;
    table_id_ = table_id;

    curr_pot = 0;
    curr_co_idx = 0;
    hands_counter = 0;
    show_down_counter = 0;
    two_players_in_counter = 0;
    three_players_in_counter = 0;
    four_players_in_counter = 0;

    cut_off_in = dealer_in = small_blind_in = big_blind_in = false;

    hands_stats = map<HandRank, int>();
    for (int rank = HighCArd; rank != StraightFlush + 1; rank++) {
        auto hand_rank = static_cast<HandRank >(rank);
        hands_stats[hand_rank] = 0;
    }

    for (int scenario = BigBlind_In; scenario != CutOff_Dealer_SmallBlind_BigBlind_In + 1; scenario++) {
        auto curr_scenario = static_cast<Scenarios >(scenario);
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
}

void PokerTable::Round() {
    EndRound();

    Deck deck = Deck();
    deck.Shuffle();

    for(int i=0 ; i<TABLE_SIZE ; i++){    // posts blinds
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
        Action action = players[(curr_co_idx+i)%TABLE_SIZE].GetAction(previous_action); // starts plays from CO
        if(action == AllIn){
            previous_action = static_cast<PreviousAction >(previous_action+1);
            if(players[(curr_co_idx+i)%TABLE_SIZE].GetPosition() == CutOff || players[(curr_co_idx+i)%TABLE_SIZE].GetPosition() == Dealer){
                players[(curr_co_idx+i)%TABLE_SIZE].UpdateMoney(-all_in_);
                curr_pot += all_in_;
                if(players[(curr_co_idx+i)%TABLE_SIZE].GetPosition() == CutOff){
                    cut_off_in = true;
                } else{
                    dealer_in = true;
                }
            }
            else if(players[(curr_co_idx+i)%TABLE_SIZE].GetPosition() == SmallBlind){
                players[(curr_co_idx+i)%TABLE_SIZE].UpdateMoney(-all_in_ + small_blind_);
                curr_pot += (+all_in_ - small_blind_);
                small_blind_in = true;
            }
            else if(players[(curr_co_idx+i)%TABLE_SIZE].GetPosition() == BigBlind){
                players[(curr_co_idx+i)%TABLE_SIZE].UpdateMoney(-all_in_ + big_blind_);
                curr_pot += (+all_in_ - big_blind_);
                big_blind_in = true;
            }
        }
    }

    // deal community cards
    deck.DealCard();
    community_cards.push_back(deck.DealCard());
    community_cards.push_back(deck.DealCard());
    community_cards.push_back(deck.DealCard());
    deck.DealCard();
    community_cards.push_back(deck.DealCard());
    deck.DealCard();
    community_cards.push_back(deck.DealCard());

    for(int i=0 ; i<TABLE_SIZE ; i++) {      // evaluate hands
        players[i].EvaluateHand(community_cards);
    }

    sort(players.begin(), players.end(), SortByHand);
    int winning_counter = 0;
    for(int i=0; i<TABLE_SIZE; i++){
        if(players[0] == players[i])
            winning_counter ++;
    }
    double wins_money = (double)(curr_pot)/winning_counter;
    for(int i=0; i<winning_counter; i++){
        players[i].UpdateMoney(wins_money);
    }

    // TODO: put stats update in function
    bool sanity_check = false;
    if(cut_off_in){
        if(dealer_in){
            if(small_blind_in){
                if(big_blind_in){
                    scenarios_stats[CutOff_Dealer_SmallBlind_BigBlind_In] ++;
                    sanity_check = true;
                } else{
                    scenarios_stats[CutOff_Dealer_SmallBlind_In] ++;
                    sanity_check = true;
                }
            }
            else{
                if(big_blind_in){
                    scenarios_stats[CutOff_Dealer_BigBlind_In] ++;
                    sanity_check = true;
                } else{
                    scenarios_stats[CutOff_Dealer_In] ++;
                    sanity_check = true;
                }
            }
        }
        else{           // cut off in dealer out
            if(small_blind_in){
                if(big_blind_in){
                    scenarios_stats[CutOff_SmallBlind_BigBlind_In] ++;
                    sanity_check = true;
                } else{
                    scenarios_stats[CutOff_SmallBlind_In] ++;
                    sanity_check = true;
                }
            }
            else{
                if(big_blind_in){
                    scenarios_stats[CutOff_BigBlind_In] ++;
                    sanity_check = true;
                } else{
                    scenarios_stats[CutOff_In] ++;
                    sanity_check = true;
                }
            }
        }
    }
    else{                   // cut_off_folds
        if(dealer_in){
            if(small_blind_in){
                if(big_blind_in){
                    scenarios_stats[Dealer_SmallBlind_BigBlind_In] ++;
                    sanity_check = true;
                } else{
                    scenarios_stats[Dealer_SmallBlind_In] ++;
                    sanity_check = true;
                }
            }
            else{
                if(big_blind_in){
                    scenarios_stats[Dealer_BigBlind_In] ++;
                    sanity_check = true;
                } else{
                    scenarios_stats[Dealer_In] ++;
                    sanity_check = true;
                }
            }
        }
        else{           // cut off in dealer out
            if(small_blind_in){
                if(big_blind_in){
                    scenarios_stats[SmallBlind_BigBlind_In] ++;
                    sanity_check = true;
                } else{
                    scenarios_stats[SmallBlind_In] ++;
                    sanity_check = true;
                }
            }
            else{
                scenarios_stats[BigBlind_In] ++;
                sanity_check = true;
            }
        }
    }

    assert(sanity_check);

    if(previous_action == TwoRaises){
        two_players_in_counter ++;
        show_down_counter ++;
    } else if(previous_action == ThreeRaises){
        three_players_in_counter ++;
        show_down_counter ++;
    } else if(previous_action == FourRaises){
        four_players_in_counter ++;
        show_down_counter ++;
    } else if(previous_action == OneRaise){    }
    else {
        assert(false);
    }

    hands_counter++;
    double total_money = 0;
    for(auto x:players){
        total_money += x.GetMoney();
        UpdateHandsStats(hands_stats, x.GetPlayerBestHand());
    }
    assert(abs(total_money) <= 0.1 && "-ASSERT- none zero sum of money, ");

    sort(players.begin(), players.end(), SortByID);

}

static void UpdateHandsStats(map<HandRank, int > & ranks_stats, PokerHand hand ){
    switch (hand.GetRank()) {
        case HighCArd:
            ranks_stats[HighCArd] ++;
            break;
        case Pair:
            ranks_stats[Pair] ++;
            break;
        case TwoPairs:
            ranks_stats[TwoPairs] ++;
            break;
        case Trips:
            ranks_stats[Trips] ++;
            break;
        case Straight:
            ranks_stats[Straight] ++;
            break;
        case Flush:
            ranks_stats[Flush] ++;
            break;
        case FullHouse:
            ranks_stats[FullHouse] ++;
            break;
        case Quads:
            ranks_stats[Quads] ++;
            break;
        case StraightFlush:
            ranks_stats[StraightFlush] ++;
            break;
        case UnSet:
            assert(false);
    }
}

static bool SortByID(PokerPlayer p1, PokerPlayer p2){
    return p1.GetID() < p2.GetID();
}

static bool SortByHand(PokerPlayer p1, PokerPlayer p2){
    return p1 > p2;
}

void PokerTable::EndRound() {
    curr_pot = 0;
    //curr_co_idx = (curr_co_idx + 1) % TABLE_SIZE;
    for(int i=0;i<TABLE_SIZE;i++){
        //players[i].UpdatePosition();
        players[i].MockHand();
        players[i].UnSetAction();
    }
    previous_action = Empty;
    community_cards.clear();

    cut_off_in = dealer_in = small_blind_in = big_blind_in = false;

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
    if(abs(100-percentage_sum) > 0.01){
        cout << "==========================" << endl;
        cout << percentage_sum << endl;
        cout << "==========================" << endl;
        exit(8);
    }
    assert(abs(100-percentage_sum) < 0.01);
    stats_string += "\n";
    stats_string += "\nShow Downs Frequency: "+to_string(100*(double)(show_down_counter)/iteration);
    stats_string += "\nTwo Players in Frequency: "+to_string(100*(double)(two_players_in_counter)/iteration);
    stats_string += "\nThree Players in Frequency: "+to_string(100*(double)(three_players_in_counter)/iteration);
    stats_string += "\nFour Players in Frequency: "+to_string(100*(double)(four_players_in_counter)/iteration);
    return stats_string;
}

string PokerTable::ToString() const{        //TODO: change printing format to something more nice
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

std::ostream& operator<<(std::ostream& os, const PokerTable& table){
    os << table.ToString();
    return os;
}
