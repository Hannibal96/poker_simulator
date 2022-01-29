#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include "Card.h"
#include "VectorStrategy.h"
#include "PokerPlayer.h"
#include "PokerTable.h"

#define REPEATS 900000000
#define PRINTS  150000000

using namespace std;

VectorStrategy GetStrategyVector(int range);
map<string, pair<int, int>> ParseInput(int argc, char *argv[]);


int main(int argc, char *argv[]) {

    string name1 = "Player1";
    string name2 = "Player2";
    string name3 = "Player3";
    string name4 = "Player4";

    vector<VectorStrategy> cutoff_possible_strategy, dealer_possible_strategy,
                small_blind_possible_strategy, big_blind_possible_strategy;

    map<string, pair<int, int>> pos_range_map = ParseInput(argc, argv);

    for(auto const& entry : pos_range_map){
        int low_range = entry.second.first, high_range = entry.second.second;
        string position = entry.first;

        vector<VectorStrategy> temp_strategy_vectors = vector<VectorStrategy>();
        for(int range = low_range ; range <= high_range ; range += 5){
            temp_strategy_vectors.push_back(GetStrategyVector(range));
        }

        if(position == "CO"){
            cutoff_possible_strategy = vector<VectorStrategy>(temp_strategy_vectors);
        }
        else if(position == "DE"){
            dealer_possible_strategy = vector<VectorStrategy>(temp_strategy_vectors);
        }
        else if(position == "SB"){
            small_blind_possible_strategy = vector<VectorStrategy>(temp_strategy_vectors);
        }
        else if(position == "BB"){
            big_blind_possible_strategy = vector<VectorStrategy>(temp_strategy_vectors);
        }
        else{
            assert(false);
        }

    }

    vector<double> factors = {0.1, 0.5, 1.0, 2.0, 5.0, 10.0};
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(0.0, 1.0);
    double r = dis(gen);
    auto idx = (unsigned int) (6 * r);

    double factor = factors[idx];

    double bb = 0.25 * factor,
            sb = 0.1 * factor,
            all_in = 2.0 * factor;


    double epsilon_co = 0.1,
            epsilon_de = 0.1,
            epsilon_sb = 0.1,
            epsilon_bb = 0.1;


    cout << "==============================================================================================================" << endl;
    cout << "====================================== Factor:" << factor << "==========================================================" << endl;
    cout << "====================================== Epsilons:" << epsilon_co << ", " << epsilon_de << ", " << epsilon_sb << ", " << epsilon_bb  << "==========================================================" << endl;
    cout << "==============================================================================================================" << endl;

    for(auto co_stg:cutoff_possible_strategy){
        for(auto de_stg:dealer_possible_strategy){
            for(auto sb_stg:small_blind_possible_strategy){
                for(auto bb_stg:big_blind_possible_strategy){

                    PokerPlayer player1 = PokerPlayer(name1, 11, co_stg, CutOff, epsilon_co);
                    PokerPlayer player2 = PokerPlayer(name2, 12, de_stg, Dealer, epsilon_de);
                    PokerPlayer player3 = PokerPlayer(name3, 13, sb_stg, SmallBlind, epsilon_sb);
                    PokerPlayer player4 = PokerPlayer(name4, 14, bb_stg, BigBlind, epsilon_bb);
                    PokerTable table = PokerTable(player1, player2, player3, player4, bb, sb, all_in ,10, false, 20);

                    for(int i=0; i<REPEATS; i++){

                        table.Round();

                        if((i+1)%PRINTS == 0 || i == 0) {
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

VectorStrategy GetStrategyVector(int range){
    VectorStrategy stg;
    switch (range) {
        case 0: {
            vector<int> stg_00_unst = vector<int>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            vector<int> stg_00_suit = vector<int>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            string name_00 = "ZeroPercent";
            stg = VectorStrategy(stg_00_unst, stg_00_suit, name_00);
            break;
        }
        case 5: {
            vector<int> stg_05_unst = vector<int>{0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2};
            vector<int> stg_05_suit = vector<int>{0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 3};
            string name_05 = "FivePercent";
            stg = VectorStrategy(stg_05_unst, stg_05_suit, name_05);
            break;
        }
        case 10: {
            vector<int> stg_10_unst = vector<int>{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 5};
            vector<int> stg_10_suit = vector<int>{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 4};
            string name_10 = "TenPercent";
            stg = VectorStrategy(stg_10_unst, stg_10_suit, name_10);
            break;
        }
        case 15: {
            vector<int> stg_15_unst = vector<int>{0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2, 4};
            vector<int> stg_15_suit = vector<int>{0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 3, 6};
            string name_15 = "FifteenPercent";
            stg = VectorStrategy(stg_15_unst, stg_15_suit, name_15);
            break;
        }
        case 20: {
            vector<int> stg_20_unst = vector<int>{0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 3, 5};
            vector<int> stg_20_suit = vector<int>{0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2, 2, 8};
            string name_20 = "TwentyPercent";
            stg = VectorStrategy(stg_20_unst, stg_20_suit, name_20);
            break;
        }
        case 25: {
            vector<int> stg_25_unst = vector<int>{0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 3, 7};
            vector<int> stg_25_suit = vector<int>{0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 2, 3, 7};
            string name_25 = "TwentyfivePercent";
            stg = VectorStrategy(stg_25_unst, stg_25_suit, name_25);
            break;
        }
        case 30: {
            vector<int> stg_30_unst = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 7};
            vector<int> stg_30_suit = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 4, 5};
            string name_30 = "ThirtyPercent";
            stg = VectorStrategy(stg_30_unst, stg_30_suit, name_30);
            break;
        }
        case 35: {
            vector<int> stg_35_unst = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 4, 5};
            vector<int> stg_35_suit = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 7, 1};
            string name_35 = "ThirtyfivePercent";
            stg = VectorStrategy(stg_35_unst, stg_35_suit, name_35);
            break;
        }
        case 40: {
            vector<int> stg_40_unst = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 5, 3};
            vector<int> stg_40_suit = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 5, 1};
            string name_40 = "FourtyPercent";
            stg = VectorStrategy(stg_40_unst, stg_40_suit, name_40);
            break;
        }
        case 45: {
            vector<int> stg_45_unst = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 7, 1};
            vector<int> stg_45_suit = vector<int>{1, 0, 0, 0, 0, 0, 0, 1, 1, 2, 6, 1, 1};
            string name_45 = "FourtyfivePercent";
            stg = VectorStrategy(stg_45_unst, stg_45_suit, name_45);
            break;
        }
        case 50: {
            vector<int> stg_50_unst = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 1, 2, 4, 4, 1};
            vector<int> stg_50_suit = vector<int>{1, 0, 0, 0, 0, 0, 1, 1, 1, 3, 4, 1, 1};
            string name_50 = "FiftyPercent";
            stg = VectorStrategy(stg_50_unst, stg_50_suit, name_50);
            break;
        }
        case 55: {
            vector<int> stg_55_unst = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 2, 2, 4, 3, 1};
            vector<int> stg_55_suit = vector<int>{1, 0, 0, 0, 1, 0, 1, 1, 1, 4, 2, 1, 1};
            string name_55 = "FiftyfivePercent";
            stg = VectorStrategy(stg_55_unst, stg_55_suit, name_55);
            break;
        }
        case 60: {
            vector<int> stg_60_unst = vector<int>{1, 0, 0, 0, 0, 0, 0, 1, 1, 2, 6, 1, 1};
            vector<int> stg_60_suit = vector<int>{1, 0, 0, 1, 1, 0, 1, 1, 2, 3, 1, 1, 1};
            string name_60 = "SixtyPercent";
            stg = VectorStrategy(stg_60_unst, stg_60_suit, name_60);
            break;
        }
        case 65: {
            vector<int> stg_65_unst = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1};
            vector<int> stg_65_suit = vector<int>{1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 9};
            string name_65 = "SixtyfivePercent";
            stg = VectorStrategy(stg_65_unst, stg_65_suit, name_65);
            break;
        }
        case 70: {
            vector<int> stg_70_unst = vector<int>{1, 0, 0, 0, 0, 0, 1, 0, 2, 3, 4, 1, 1};
            vector<int> stg_70_suit = vector<int>{1, 0, 1, 1, 0, 1, 0, 1, 4, 1, 1, 1, 1};
            string name_70 = "SeventyPercent";
            stg = VectorStrategy(stg_70_unst, stg_70_suit, name_70);
            break;
        }
        case 75:{
            vector<int> stg_75_unst = vector<int>{1, 0, 0, 0, 1, 0, 1, 1, 1, 4, 2, 1, 1};
            vector<int> stg_75_suit = vector<int>{1, 0, 2, 1, 0, 1, 0, 3, 1, 1, 1, 1, 1};
            string name_75 = "SeventyfivePercent";
            stg = VectorStrategy(stg_75_unst, stg_75_suit, name_75);
            break;
        }
        case 100:{
            vector<int> stg_100_unst = vector<int>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
            vector<int> stg_100_suit = vector<int>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
            string name_100 = "HundredPercent";
            stg = VectorStrategy(stg_100_unst, stg_100_suit, name_100);
            break;
        }
        default:
            cout << "-E- Can't init " << range << " Strategy vector!" << endl;
            exit(1);
    }
    return stg;
}

map<string, pair<int, int>> ParseInput(int argc, char *argv[]){
    if(argc == 1){
        cout << "-Help:  " << endl <<
             "    input format --CO MIN-RANGE-CO_MAX-RANGE-CO --DE MIN-DE_MAX-DE --SB MIN-SB_MAX-SB --BB MIN-BB_MAX-BB" << endl;
    }
    map<string, pair<int, int>> pos_range_map = map<string, pair<int, int>>();
    string delim = "_";
    string s, pos;
    for(int i = 1; i < argc ; i++){
        s = argv[i];
        if(s == "--CO" || s == "--DE" || s == "--SB" || s == "--BB"){
            pos = s.substr(2,4);
            s = argv[++i];
            int low_int = atoi(s.substr(0, s.find(delim)).c_str());
            s = s.erase(0, s.find(delim)+delim.length());
            int high_int = atoi(s.substr(0, s.find(delim)).c_str());
            pos_range_map[pos] = pair<int, int>(low_int, high_int);
        }
        else{
            cout << "--Wrong Input:  " << endl <<
                 "    input format --CO MIN-RANGE-CO_MAX-RANGE-CO --DE MIN-DE_MAX-DE --SB MIN-SB_MAX-SB --BB MIN-BB_MAX-BB" << endl;
        }
    }
    return pos_range_map;
}

