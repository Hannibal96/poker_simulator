#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include "Card.h"
#include "VectorStrategy.h"
#include "PokerPlayer.h"
#include "PokerTable.h"


using namespace std;

tuple<map<Situation, vector<double>>, map<string, double>> ParseInputStrategy(int argc, char *argv[]);
Strategy BuildStrategy(map<Situation, vector<double>> map, Position pos);
VectorStrategy GetStrategyVector(int players, int range);
VectorStrategy GetStrategyVectorP1(int range);
VectorStrategy GetStrategyVectorP2(int range);
VectorStrategy GetStrategyVectorP3(int range);
VectorStrategy GetStrategyVectorP4(int range);


int main(int argc, char *argv[]) {

    string name1 = "Player1";
    string name2 = "Player2";
    string name3 = "Player3";
    string name4 = "Player4";

    Strategy co_stg, de_stg, sb_stg, bb_stg;

    //tuple<map<Situation, vector<double>>, map<string, double>> tuple = ParseInputStrategy(argc, argv);

    //map<Situation, vector<double>> situation_strategy_map;
    //map<string, double> parameters_map;

    auto [situation_strategy_map, parameters_map] = ParseInputStrategy(argc, argv);

    co_stg = BuildStrategy(situation_strategy_map, CutOff);
    de_stg = BuildStrategy(situation_strategy_map, Dealer);
    sb_stg = BuildStrategy(situation_strategy_map, SmallBlind);
    bb_stg = BuildStrategy(situation_strategy_map, BigBlind);

    PokerPlayer player1 = PokerPlayer(name1, 11, co_stg, CutOff);
    PokerPlayer player2 = PokerPlayer(name2, 12, de_stg, Dealer);
    PokerPlayer player3 = PokerPlayer(name3, 13, sb_stg, SmallBlind);
    PokerPlayer player4 = PokerPlayer(name4, 14, bb_stg, BigBlind);

    double factor = 10*((double)rand()+1)/RAND_MAX;
    factor = 1;

    double bb = parameters_map["bb"] * factor,
            sb = parameters_map["sb"] * factor,
            all_in = parameters_map["all_in"] * factor;

    int print = int(parameters_map["print"]),
        repeats = int(parameters_map["repeats"]),
        rounds = int(parameters_map["rounds"]);

    PokerTable table = PokerTable(player1, player2, player3, player4, bb, sb, all_in ,10, false, repeats);

    cout << "==============================================================================================================" << endl;
    cout << "================== Repeats: " << repeats << " === Prints: " << print << " =====================================================" << endl;
    cout << "================== All_in: " << all_in << " === BB: " << bb << " === SB: " << sb << " === Factor: " << factor << " ==========================================" << endl;
    cout << "==============================================================================================================" << endl;

    for(int i=0; i<rounds; i++){

        table.Round();

        if((i+1)%print == 0 || i == 0) {
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

    return 0;
}

Strategy BuildStrategy(map<Situation, vector<double>> map, Position pos){

    Strategy res_strategy = Strategy();

    vector<Situation> situation_to_iterate = vector<Situation>();
    if(pos == CutOff){
        situation_to_iterate.push_back(CO);
    }
    else if(pos == Dealer){
        situation_to_iterate.push_back(DE);
        situation_to_iterate.push_back(DE_CO);
    }
    else if(pos == SmallBlind){
        situation_to_iterate.push_back(SB);
        situation_to_iterate.push_back(SB_CO);
        situation_to_iterate.push_back(SB_DE);
        situation_to_iterate.push_back(SB_CO_DE);
    }
    else if(pos == BigBlind){
        situation_to_iterate.push_back(BB_CO);
        situation_to_iterate.push_back(BB_DE);
        situation_to_iterate.push_back(BB_SB);
        situation_to_iterate.push_back(BB_CO_DE);
        situation_to_iterate.push_back(BB_CO_SB);
        situation_to_iterate.push_back(BB_DE_SB);
        situation_to_iterate.push_back(BB_CO_DE_SB);
    }
    else{
        cout << "-E- impossible position BuildStrategy "<< endl;
        exit(1);
    }

    bool sanity_flag = false;
    for(auto sit: situation_to_iterate){
        vector<double> curr_params = map[sit];
        if(curr_params.size() == 5){  // Bandit
            double start_epsilon = curr_params[0], final_epsilon = curr_params[1] , epsilon_decay = curr_params[2]
                    , epsilon_tie_break = curr_params[3];
            int decay_cycle = (int)curr_params[4];
            res_strategy.UpdateBandit(sit, start_epsilon, final_epsilon, epsilon_decay, epsilon_tie_break, decay_cycle);

            sanity_flag = true;
        }
        else if(curr_params.size() == 2){ // Vector
            int players = (int)curr_params[0], range = (int)curr_params[1];
            VectorStrategy curr_vector = GetStrategyVector(players, range);
            res_strategy.UpdateVector(sit, curr_vector);
        }
        else{
            if(!sanity_flag) {
                cout << "-E- impossible number of parameters BuildStrategy " << endl;
                exit(1);
            }
        }
    }

    return res_strategy;
}

VectorStrategy GetStrategyVectorP1(int range){
    // open player
    VectorStrategy stg;

    switch (range) {
        case 0: {
            vector<int> stg_00_unst = vector<int>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            vector<int> stg_00_suit = vector<int>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            string name_00 = "P1_ZeroPercent";
            stg = VectorStrategy(stg_00_unst, stg_00_suit, name_00);
            break;
        }
        case 5: {
            vector<int> stg_05_unst = vector<int>{0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1};
            vector<int> stg_05_suit = vector<int>{0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 4};
            string name_05 = "P1_FivePercent";
            stg = VectorStrategy(stg_05_unst, stg_05_suit, name_05);
            break;
        }
        case 10: {
            vector<int> stg_10_unst = vector<int>{0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 2};
            vector<int> stg_10_suit = vector<int>{0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 4};
            string name_10 = "P1_TenPercent";
            stg = VectorStrategy(stg_10_unst, stg_10_suit, name_10);
            break;
        }
        case 15: {
            vector<int> stg_15_unst = vector<int>{0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3};
            vector<int> stg_15_suit = vector<int>{0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 3};
            string name_15 = "P1_FifteenPercent";
            stg = VectorStrategy(stg_15_unst, stg_15_suit, name_15);
            break;
        }
        case 20: {
            vector<int> stg_20_unst = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 3};
            vector<int> stg_20_suit = vector<int>{1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 8};
            string name_20 = "P1_TwentyPercent";
            stg = VectorStrategy(stg_20_unst, stg_20_suit, name_20);
            break;
        }
        case 25: {
            vector<int> stg_25_unst = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 2, 5};
            vector<int> stg_25_suit = vector<int>{1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 8};
            string name_25 = "P1_TwentyfivePercent";
            stg = VectorStrategy(stg_25_unst, stg_25_suit, name_25);
            break;
        }
        case 30: {
            vector<int> stg_30_unst = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 8};
            vector<int> stg_30_suit = vector<int>{1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 2, 6};
            string name_30 = "P1_ThirtyPercent";
            stg = VectorStrategy(stg_30_unst, stg_30_suit, name_30);
            break;
        }
        case 35: {
            vector<int> stg_35_unst = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 3, 7};
            vector<int> stg_35_suit = vector<int>{1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 3, 4, 2};
            string name_35 = "P1_ThirtyfivePercent";
            stg = VectorStrategy(stg_35_unst, stg_35_suit, name_35);
            break;
        }
        case 40: {
            vector<int> stg_40_unst = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 7};
            vector<int> stg_40_suit = vector<int>{1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 3, 4, 1};
            string name_40 = "P1_FourtyPercent";
            stg = VectorStrategy(stg_40_unst, stg_40_suit, name_40);
            break;
        }
        case 45: {
            vector<int> stg_45_unst = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 6, 3};
            vector<int> stg_45_suit = vector<int>{1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 3, 3, 1};
            string name_45 = "P1_FourtyfivePercent";
            stg = VectorStrategy(stg_45_unst, stg_45_suit, name_45);
            break;
        }
        case 50: {
            vector<int> stg_50_unst = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1, 7, 1};
            vector<int> stg_50_suit = vector<int>{1, 0, 0, 0, 1, 0, 1, 1, 1, 2, 4, 1, 1};
            string name_50 = "P1_FiftyPercent";
            stg = VectorStrategy(stg_50_unst, stg_50_suit, name_50);
            break;
        }
        case 55: {
            vector<int> stg_55_unst = vector<int>{1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 3, 5, 1};
            vector<int> stg_55_suit = vector<int>{1, 0, 0, 1, 0, 1, 0, 1, 1, 3, 3, 1, 1};
            string name_55 = "P1_FiftyfivePercent";
            stg = VectorStrategy(stg_55_unst, stg_55_suit, name_55);
            break;
        }
        case 100:{
            vector<int> stg_100_unst = vector<int>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
            vector<int> stg_100_suit = vector<int>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
            string name_100 = "P1_HundredPercent";
            stg = VectorStrategy(stg_100_unst, stg_100_suit, name_100);
            break;
        }
        default:
            cout << "-E- Can't init " << range << " Strategy vector!" << endl;
            exit(1);
    }

    return stg;
}

VectorStrategy GetStrategyVectorP2(int range){
    // defend player
    VectorStrategy stg;
    switch (range) {
        case 0: {
            vector<int> stg_00_unst = vector<int>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            vector<int> stg_00_suit = vector<int>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            string name_00 = "P1_ZeroPercent";
            stg = VectorStrategy(stg_00_unst, stg_00_suit, name_00);
            break;
        }
        case 5: {
            vector<int> stg_05_unst = vector<int>{0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1};
            vector<int> stg_05_suit = vector<int>{0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 3};
            string name_05 = "P1_FivePercent";
            stg = VectorStrategy(stg_05_unst, stg_05_suit, name_05);
            break;
        }
        case 10: {
            vector<int> stg_10_unst = vector<int>{0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4};
            vector<int> stg_10_suit = vector<int>{0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 4};
            string name_10 = "P1_TenPercent";
            stg = VectorStrategy(stg_10_unst, stg_10_suit, name_10);
            break;
        }
        case 15: {
            vector<int> stg_15_unst = vector<int>{0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 5};
            vector<int> stg_15_suit = vector<int>{0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 3, 6};
            string name_15 = "P1_FifteenPercent";
            stg = VectorStrategy(stg_15_unst, stg_15_suit, name_15);
            break;
        }
        case 20: {
            vector<int> stg_20_unst = vector<int>{0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4};
            vector<int> stg_20_suit = vector<int>{0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 8};
            string name_20 = "P1_TwentyPercent";
            stg = VectorStrategy(stg_20_unst, stg_20_suit, name_20);
            break;
        }
        case 25: {
            vector<int> stg_25_unst = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 7};
            vector<int> stg_25_suit = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 8};
            string name_25 = "P1_TwentyfivePercent";
            stg = VectorStrategy(stg_25_unst, stg_25_suit, name_25);
            break;
        }
        case 30: {
            vector<int> stg_30_unst = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 8};
            vector<int> stg_30_suit = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 5, 4};
            string name_30 = "P1_ThirtyPercent";
            stg = VectorStrategy(stg_30_unst, stg_30_suit, name_30);
            break;
        }
        case 35: {
            vector<int> stg_35_unst = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 4, 6};
            vector<int> stg_35_suit = vector<int>{1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 7, 1};
            string name_35 = "P1_ThirtyfivePercent";
            stg = VectorStrategy(stg_35_unst, stg_35_suit, name_35);
            break;
        }
        case 40: {
            vector<int> stg_40_unst = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 4, 5};
            vector<int> stg_40_suit = vector<int>{1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 3, 4, 1};
            string name_40 = "P1_FourtyPercent";
            stg = VectorStrategy(stg_40_unst, stg_40_suit, name_40);
            break;
        }
        case 45: {
            vector<int> stg_45_unst = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 5, 3};
            vector<int> stg_45_suit = vector<int>{1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 5, 2, 1};
            string name_45 = "P1_FourtyfivePercent";
            stg = VectorStrategy(stg_45_unst, stg_45_suit, name_45);
            break;
        }
        case 50: {
            vector<int> stg_50_unst = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 5, 1};
            vector<int> stg_50_suit = vector<int>{1, 0, 0, 1, 0, 0, 1, 1, 1, 2, 4, 1, 1};
            string name_50 = "P1_FiftyPercent";
            stg = VectorStrategy(stg_50_unst, stg_50_suit, name_50);
            break;
        }
        case 55: {
            vector<int> stg_55_unst = vector<int>{1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 4, 4, 1};
            vector<int> stg_55_suit = vector<int>{1, 0, 0, 0, 1, 0, 1, 1, 1, 4, 2, 1, 1};
            string name_55 = "P1_FiftyfivePercent";
            stg = VectorStrategy(stg_55_unst, stg_55_suit, name_55);
            break;
        }
        case 100:{
            vector<int> stg_100_unst = vector<int>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
            vector<int> stg_100_suit = vector<int>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
            string name_100 = "P1_HundredPercent";
            stg = VectorStrategy(stg_100_unst, stg_100_suit, name_100);
            break;
        }
        default:
            cout << "-E- Can't init " << range << " Strategy vector!" << endl;
            exit(1);
    }
    return stg;
}

VectorStrategy GetStrategyVector(int players, int range){
    switch (players) {
        case 1:
            return GetStrategyVectorP1(range);
            break;
        case 2:
            return GetStrategyVectorP2(range);
            break;
        default:
            cout << "-E- Can't init " << range << " Strategy vector!" << endl;
            exit(1);
    }
}

tuple<map<Situation, vector<double>>, map<string, double>> ParseInputStrategy(int argc, char *argv[]){
    if(argc == 1){
        cout << "-Help:  " << endl <<
             "    input format: --Situation Stragey "
             "    For example:"
             "    ./sim.exe --CO Vector_1_25 --DE Bandit_<start>_<end>_<decay>_<tie_break>_<cycle> --DE_CO Vector_1_15 --SB Vector_1_60 --SB_CO Vector_1_15"
             " --SB_DE Bandit_0.3_0.05_0.9_1000 --SB_CO_DE Vector_2_20 --BB_CO Vector_1_30 --BB_DE Vector_1_30 --BB_SB Vector_1_30"
             " --BB_CO_DE Vector_2_30 --BB_CO_SB Vector_2_30 --BB_DE_SB Vector_2_30 --BB_CO_DE_SB Vector_3_35" << endl;
        exit(0);
    }

    map<string, Situation> string_situation_map = map<string, Situation>();
    string_situation_map["CO"] = CO; string_situation_map["DE"] = DE; string_situation_map["DE_CO"] = DE_CO;
    string_situation_map["SB"] = SB; string_situation_map["SB_CO"] = SB_CO; string_situation_map["SB_DE"] = SB_DE; string_situation_map["SB_CO_DE"] = SB_CO_DE;
    string_situation_map["BB_CO"] = BB_CO; string_situation_map["BB_DE"] = BB_DE; string_situation_map["BB_SB"] = BB_SB;
    string_situation_map["BB_CO_DE"] = BB_CO_DE; string_situation_map["BB_CO_SB"] = BB_CO_SB; string_situation_map["BB_DE_SB"] = BB_DE_SB;
    string_situation_map["BB_CO_DE_SB"] = BB_CO_DE_SB;

    map<Situation, vector<double>> situation_strategy_map = map<Situation, vector<double> >();
    map<string, double> parameters_map = map<string , double>();
    string delim = "_";
    string s, pos, type;
    double num_players, range;
    double start_epsilon, final_epsilon, epsilon_decay, decay_cycle, epsilon_tie_break;

    for(int i = 1; i < argc ; i++){
        s = argv[i];
        if(s == "--CO" || s == "--DE" || s == "--DE_CO"  || s == "--SB"  || s == "--SB_CO"  || s == "--SB_DE"  ||
                s == "--SB_CO_DE"  || s == "--BB_CO"  || s == "--BB_DE"  || s == "--BB_SB"  || s == "--BB_CO_DE" ||
                s == "--BB_CO_SB" || s == "--BB_DE_SB" || s == "--BB_CO_DE_SB" ){

            s = s.substr(2,s.length());
            Situation curr_situation = string_situation_map[s];
            s = argv[++i];
            type = s.substr(0, s.find(delim));
            s = s.erase(0, s.find(delim)+delim.length());

            if(type == "Vector"){
                num_players = atoi(s.substr(0, s.find(delim)).c_str());
                s = s.erase(0, s.find(delim)+delim.length());
                range = atoi(s.substr(0, s.find(delim)).c_str());

                situation_strategy_map[curr_situation] = {num_players, range};
            }

            else if(type == "Bandit"){
                start_epsilon = atof(s.substr(0, s.find(delim)).c_str());
                s = s.erase(0, s.find(delim)+delim.length());

                final_epsilon = atof(s.substr(0, s.find(delim)).c_str());
                s = s.erase(0, s.find(delim)+delim.length());

                epsilon_decay = atof(s.substr(0, s.find(delim)).c_str());
                s = s.erase(0, s.find(delim)+delim.length());

                epsilon_tie_break = atof(s.substr(0, s.find(delim)).c_str());
                s = s.erase(0, s.find(delim)+delim.length());

                decay_cycle = atoi(s.substr(0, s.find(delim)).c_str());

                situation_strategy_map[curr_situation] = {start_epsilon, final_epsilon, epsilon_decay, epsilon_tie_break, decay_cycle};
            }
        }

        else if(s == "--bb") {
            s = argv[++i];
            parameters_map["bb"] = stod(s);
        }
        else if(s == "--sb") {
            s = argv[++i];
            parameters_map["sb"] = stod(s);
        }
        else if(s == "--allin" || s == "--ALLIN" || s == "--Allin" || s == "--AllIn" ||
                s == "--all_in" || s == "--ALL_IN" || s == "--All_in" || s == "--All_In") {
            s = argv[++i];
            parameters_map["all_in"] = stod(s);
        }
        else if(s == "--print" || s == "--Print" || s == "--PRINT"  ) {
            s = argv[++i];
            parameters_map["print"] = stod(s);
        }
        else if(s == "--repeat" || s == "--Repeat" || s == "--REPEAT" ||
                s == "--repeats" || s == "--Repeats" || s == "--REPEATS") {
            s = argv[++i];
            parameters_map["repeats"] = stod(s);
        }

        else if(s == "--rounds" || s == "--Rounds" || s == "--ROUNDS" ) {
            s = argv[++i];
            parameters_map["rounds"] = stod(s);
        }


        else{
            cout << "--Wrong Input:  " << endl <<
                 "    input format --CO MIN-RANGE-CO_MAX-RANGE-CO --DE MIN-DE_MAX-DE --SB MIN-SB_MAX-SB --BB MIN-BB_MAX-BB"
                 << endl << "--AllIn 2.0 --BB 0.25 --SB 0.1 --print 1000000 --repeat 10000000" << endl;
        }
    }

    tuple<map<Situation, vector<double>>, map<string, double>> tuple =
            make_tuple(situation_strategy_map, parameters_map);

    return tuple;
}



