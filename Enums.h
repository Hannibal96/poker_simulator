//
// Created by Neria on 07/07/2020.
//

#ifndef POKER_SIMULATOR_ENUMS_H
#define POKER_SIMULATOR_ENUMS_H


#define TABLE_SIZE 4

enum Position{BigBlind, SmallBlind, Dealer, CutOff};
enum Suit{Heart , Diamond, Spade, Club};
enum Value{Duce = 2, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace};
enum PreviousAction{Empty, OneRaise, TwoRaises, ThreeRaises, FourRaises};
enum Action{NotAct, Fold, AllIn};
enum HandRank{UnSet, HighCArd, Pair, TwoPairs, Trips, Straight, Flush, FullHouse, Quads, StraightFlush};
enum Scenarios{BigBlind_In, CutOff_In, Dealer_In, SmallBlind_In, CutOff_Dealer_In, CutOff_SmallBlind_In,
                CutOff_BigBlind_In, Dealer_SmallBlind_In, Dealer_BigBlind_In, SmallBlind_BigBlind_In,
                CutOff_Dealer_SmallBlind_In, CutOff_Dealer_BigBlind_In, CutOff_SmallBlind_BigBlind_In,
                Dealer_SmallBlind_BigBlind_In, CutOff_Dealer_SmallBlind_BigBlind_In};



#endif //POKER_SIMULATOR_ENUMS_H
