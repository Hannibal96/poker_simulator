//
// Created by Neria on 07/07/2020.
//

#ifndef POKER_SIMULATOR_ENUMS_H
#define POKER_SIMULATOR_ENUMS_H

using namespace std;

#include <tuple>

#define TABLE_SIZE 4

enum Position{BigBlind, SmallBlind, Dealer, CutOff};
enum Suit{Heart = 0 , Diamond, Spade, Club};
enum Value{Duce = 2, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace};
enum Action{NotAct, Fold, AllIn};
enum HandRank{UnSet, HighCArd, Pair, TwoPairs, Trips, Straight, Flush, FullHouse, Quads, StraightFlush};
enum History{BigBlind_In = 0, CutOff_In, Dealer_In, SmallBlind_In, CutOff_Dealer_In, CutOff_SmallBlind_In,
                CutOff_BigBlind_In, Dealer_SmallBlind_In, Dealer_BigBlind_In, SmallBlind_BigBlind_In,
                CutOff_Dealer_SmallBlind_In, CutOff_Dealer_BigBlind_In, CutOff_SmallBlind_BigBlind_In,
                Dealer_SmallBlind_BigBlind_In, CutOff_Dealer_SmallBlind_BigBlind_In};

enum Situation{CO=0, DE, DE_CO, SB, SB_CO, SB_DE, SB_CO_DE,
                BB_CO, BB_DE, BB_SB, BB_CO_DE, BB_CO_SB, BB_DE_SB,
                BB_CO_DE_SB};

typedef pair<Situation, int > State;            // int stands for two cards, i.e. belongs to 0..168 22=0 AA=168
typedef pair<State , Action> TableEntry;


#endif //POKER_SIMULATOR_ENUMS_H
