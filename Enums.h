//
// Created by Neria on 07/07/2020.
//

#ifndef POKER_SIMULATOR_ENUMS_H
#define POKER_SIMULATOR_ENUMS_H


#define TABLE_SIZE 4

enum Position{BigBlind, SmallBlind, Dealer, CutOff};
enum Suit{Heart , Diamond, Spade, Club};
enum Value{Duce = 2, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace};
enum PreviousAction{Empty, OneRaise, TwoRaises, ThreeRaises};
enum Action{NotAct, Fold, AllIn};
enum HandRank{UnSet, HighCArd, Pair, TwoPairs, Trips, Straight, Flush, FullHouse, Quads, StraightFlush};


#endif //POKER_SIMULATOR_ENUMS_H
