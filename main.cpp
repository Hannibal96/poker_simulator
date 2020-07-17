#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include "Card.h"
#include "Deck.h"
#include "PokerHand.h"
#include "Strategy.h"
#include "PokerPlayer.h"
#include "PokerTable.h"

#define REPEATS 10000
#define PRINTS 10000

using namespace std;

map<HandRank, string> SetRanksNames();
map<HandRank, int > SetRanksStats();
void UpdateRanksStats(map<HandRank, int > & ranks_stats, PokerHand hand );

int main(int argc, char *argv[]) {

    map<HandRank, string> ranks_names = SetRanksNames();
    map<HandRank, int > ranks_stats = SetRanksStats();

    vector<int> stg_00_unst = vector<int>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    vector<int> stg_00_suit = vector<int>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    string name_00 = "ZeroPercent";
    Strategy stg_00 = Strategy(stg_00_unst, stg_00_suit, name_00);

    vector<int> stg_05_unst = vector<int>{0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2};
    vector<int> stg_05_suit = vector<int>{0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 3};
    string name_05 = "FivePercent";
    Strategy stg_05 = Strategy(stg_05_unst, stg_05_suit, name_05);

    vector<int> stg_10_unst = vector<int>{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 5};
    vector<int> stg_10_suit = vector<int>{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 4};
    string name_10 = "TenPercent";
    Strategy stg_10 = Strategy(stg_10_unst, stg_10_suit, name_10);

    vector<int> stg_15_unst = vector<int>{0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2, 4};
    vector<int> stg_15_suit = vector<int>{0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 3, 6};
    string name_15 = "FifteenPercent";
    Strategy stg_15 = Strategy(stg_15_unst, stg_15_suit, name_15);

    vector<int> stg_20_unst = vector<int>{0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 3, 5};
    vector<int> stg_20_suit = vector<int>{0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2, 2, 8};
    string name_20 = "TwentyPercent";
    Strategy stg_20 = Strategy(stg_20_unst, stg_20_suit, name_20);

    vector<int> stg_25_unst = vector<int>{0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 3, 7};
    vector<int> stg_25_suit = vector<int>{0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 2, 3, 7};
    string name_25 = "TwentyfivePercent";
    Strategy stg_25 = Strategy(stg_25_unst, stg_25_suit, name_25);

    vector<int> stg_30_unst = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 7};
    vector<int> stg_30_suit = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 4, 5};
    string name_30 = "ThirtyPercent";
    Strategy stg_30 = Strategy(stg_30_unst, stg_30_suit, name_30);

    vector<int> stg_35_unst = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 4, 5};
    vector<int> stg_35_suit = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 7, 1};
    string name_35 = "ThirtyfivePercent";
    Strategy stg_35 = Strategy(stg_35_unst, stg_35_suit, name_35);

    vector<int> stg_40_unst = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 5, 3};
    vector<int> stg_40_suit = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 5, 1};
    string name_40 = "FourtyPercent";
    Strategy stg_40 = Strategy(stg_40_unst, stg_40_suit, name_40);

    vector<int> stg_45_unst = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 7, 1};
    vector<int> stg_45_suit = vector<int>{1, 0, 0, 0, 0, 0, 0, 1, 1, 2, 6, 1, 1};
    string name_45 = "FourtyfivePercent";
    Strategy stg_45 = Strategy(stg_45_unst, stg_45_suit, name_45);

    vector<int> stg_50_unst = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 1, 2, 4, 4, 1};
    vector<int> stg_50_suit = vector<int>{1, 0, 0, 0, 0, 0, 1, 1, 1, 3, 4, 1, 1};
    string name_50 = "FiftyPercent";
    Strategy stg_50 = Strategy(stg_50_unst, stg_50_suit, name_50);

    vector<int> stg_55_unst = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 2, 2, 4, 3, 1};
    vector<int> stg_55_suit = vector<int>{1, 0, 0, 0, 1, 0, 1, 1, 1, 4, 2, 1, 1};
    string name_55 = "FiftyfivePercent";
    Strategy stg_55 = Strategy(stg_55_unst, stg_55_suit, name_55);

    vector<int> stg_60_unst = vector<int>{1, 0, 0, 0, 0, 0, 0, 1, 1, 2, 6, 1, 1};
    vector<int> stg_60_suit = vector<int>{1, 0, 0, 1, 1, 0, 1, 1, 2, 3, 1, 1, 1};
    string name_60 = "SixtyPercent";
    Strategy stg_60 = Strategy(stg_60_unst, stg_60_suit, name_60);

    vector<int> stg_65_unst = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1};
    vector<int> stg_65_suit = vector<int>{1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 9};
    string name_65 = "SixtyfivePercent";
    Strategy stg_65 = Strategy(stg_65_unst, stg_65_suit, name_65);

    vector<int> stg_70_unst = vector<int>{1, 0, 0, 0, 0, 0, 1, 0, 2, 3, 4, 1, 1};
    vector<int> stg_70_suit = vector<int>{1, 0, 1, 1, 0, 1, 0, 1, 4, 1, 1, 1, 1};
    string name_70 = "SeventyPercent";
    Strategy stg_70 = Strategy(stg_70_unst, stg_70_suit, name_70);

    vector<int> stg_75_unst = vector<int>{1, 0, 0, 0, 1, 0, 1, 1, 1, 4, 2, 1, 1};
    vector<int> stg_75_suit = vector<int>{1, 0, 2, 1, 0, 1, 0, 3, 1, 1, 1, 1, 1};
    string name_75 = "SeventyfivePercent";
    Strategy stg_75 = Strategy(stg_75_unst, stg_75_suit, name_75);

    vector<int> stg_100_unst = vector<int>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    vector<int> stg_100_suit = vector<int>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    string name_100 = "HundredPercent";
    Strategy stg_100 = Strategy(stg_100_unst, stg_100_suit, name_100);

    string name1 = "Player1";
    string name2 = "Player2";
    string name3 = "Player3";
    string name4 = "Player4";

    vector<Strategy> cutoff_possible_strategy;
    switch (atoi(argv[1])){
        case 0:
            cutoff_possible_strategy = {stg_00};
            break;
        case 5:
            cutoff_possible_strategy = {stg_05};
            break;
        case 10:
            cutoff_possible_strategy = {stg_10};
            break;
        case 15:
            cutoff_possible_strategy = {stg_15};
            break;
        case 20:
            cutoff_possible_strategy = {stg_20};
            break;
        case 25:
            cutoff_possible_strategy = {stg_25};
            break;
        case 30:
            cutoff_possible_strategy = {stg_30};
            break;
        case 35:
            cutoff_possible_strategy = {stg_35};
            break;
        case 40:
            cutoff_possible_strategy = {stg_40};
            break;
        case 45:
            cutoff_possible_strategy = {stg_45};
            break;
        case 50:
            cutoff_possible_strategy = {stg_50};
            break;
        default:
            cout << "-E- Wrong cutoff strategy!" << endl;
            exit(1);
    }

    vector<Strategy> dealer_possible_strategy = {stg_00, stg_05, stg_10, stg_15, stg_20, stg_25, stg_30, stg_35, stg_40, stg_45, stg_50};
    vector<Strategy> small_blind_possible_strategy = {stg_00, stg_05, stg_10, stg_15, stg_20, stg_25, stg_30, stg_35, stg_40, stg_45, stg_50};
    vector<Strategy> big_blind_possible_strategy = {stg_00, stg_05, stg_10, stg_15, stg_20, stg_25, stg_30, stg_35, stg_40, stg_45, stg_50};

    for(auto co_stg:cutoff_possible_strategy){
        for(auto de_stg:dealer_possible_strategy){
            for(auto sb_stg:small_blind_possible_strategy){
                for(auto bb_stg:big_blind_possible_strategy){

                    PokerPlayer player1 = PokerPlayer(name1, 11, co_stg, CutOff);
                    PokerPlayer player2 = PokerPlayer(name2, 12, de_stg, Dealer);
                    PokerPlayer player3 = PokerPlayer(name3, 13, sb_stg, SmallBlind);
                    PokerPlayer player4 = PokerPlayer(name4, 14, bb_stg, BigBlind);
                    PokerTable table = PokerTable(player1, player2, player3, player4, 0.25, 0.1, 2.0 ,10);

                    for(int i=0; i<REPEATS; i++){
                        table.Round();
                        if((i+1)%PRINTS == 0) {
                            cout << "==============================================================================================================" << endl;
                            cout << "============================******************************************************============================" << endl;
                            cout << "==============================================================================================================" << endl;
                            cout << "cutoff strategy: " << player1.GetStrategyName() << endl;
                            cout << "delear strategy: " << player2.GetStrategyName() << endl;
                            cout << "small blind strategy: " << player3.GetStrategyName() << endl;
                            cout << "big blind strategy: " << player4.GetStrategyName() << endl;
                            cout << table << endl;
                            cout << table.GetStatsSring((i + 1)) << endl;
                        }
                    }

                }
            }
        }
    }


    return 0;
}

map<HandRank, string> SetRanksNames(){
    map<HandRank, string> ranks = { { UnSet, "UnSet" },
                                    { HighCArd, "HighCard" },
                                    { Pair, "Pair" },
                                    { TwoPairs, "TwoPairs" },
                                    { Trips, "Trips" },
                                    { Straight, "Straight" },
                                    { Flush, "Flush" },
                                    { FullHouse, "FullHouse" },
                                    { Quads, "Quads" },
                                    { StraightFlush, "StraightFlush" }, };
    return ranks;
}

map<HandRank, int > SetRanksStats() {
    map<HandRank, int> stats = map<HandRank, int>();
    for (int rank = HighCArd; rank != StraightFlush + 1; rank++) {
        auto hand_rank = static_cast<HandRank >(rank);
        stats[hand_rank] = 0;
    }
    return stats;
}

void UpdateRanksStats(map<HandRank, int > & ranks_stats, PokerHand hand ){
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

