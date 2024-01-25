//
// Created by Neria on 07/07/2020.
//

#ifndef POKER_SIMULATOR_ENUMS_H
#define POKER_SIMULATOR_ENUMS_H

using namespace std;

#include <tuple>
#include <string>
#include <map>
#define TABLE_SIZE 4

enum Position{BigBlind, SmallBlind, Dealer, CutOff};
enum Suit{Heart = 0 , Diamond, Spade, Club};
enum Value{Deuce = 2, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace};
enum Action{NotAct, Fold, AllIn};
enum HandRank{UnSet, HighCArd, Pair, TwoPairs, Trips, Straight, Flush, FullHouse, Quads, StraightFlush};
enum History{BigBlind_In = 0, CutOff_In, Dealer_In, SmallBlind_In, CutOff_Dealer_In, CutOff_SmallBlind_In,
                CutOff_BigBlind_In, Dealer_SmallBlind_In, Dealer_BigBlind_In, SmallBlind_BigBlind_In,
                CutOff_Dealer_SmallBlind_In, CutOff_Dealer_BigBlind_In, CutOff_SmallBlind_BigBlind_In,
                Dealer_SmallBlind_BigBlind_In, CutOff_Dealer_SmallBlind_BigBlind_In};

enum Situation{CO=0, DE, DE_CO, SB, SB_CO, SB_DE, SB_CO_DE,
                BB_CO, BB_DE, BB_SB, BB_CO_DE, BB_CO_SB, BB_DE_SB, BB_CO_DE_SB};

typedef pair<Situation, int > State;            // int stands for two cards, i.e. belongs to 0..168 22=0 AA=168
typedef pair<State , Action> TableEntry;

const map<Suit, uint8_t> hash_suit_converter = {{Heart, 1}, {Diamond, 2}, {Spade, 0},{Club, 3}};

const map<HandRank, string> ranks_names = { { UnSet, "UnSet" },
                                            { HighCArd, "HighCard" },
                                            { Pair, "Pair" },
                                            { TwoPairs, "TwoPairs" },
                                            { Trips, "Trips" },
                                            { Straight, "Straight" },
                                            { Flush, "Flush" },
                                            { FullHouse, "FullHouse" },
                                            { Quads, "Quads" },
                                            { StraightFlush, "StraightFlush" }, };

const map<History, string> scenarios_names = {{ BigBlind_In, "Empty-BigBlind" },
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

const map<Position, string> positions_names = {{Position::BigBlind, "BigBlind"},
                                               {Position::SmallBlind, "SmallBlind"},
                                               {Position::Dealer, "Dealer"},
                                               {Position::CutOff, "CutOff"}, };

const map<string, Position> pos_map = {{"CutOff", Position::CutOff},
                                       {"Dealer", Position::Dealer},
                                       {"SmallBlind", Position::SmallBlind},
                                       {"BigBlind", Position::BigBlind}};

const map<Action, string> action_names = {{Action::Fold, "Fold"},
                                          {Action::AllIn, "AllIn"}};

const map<Situation, string> situation_names = {
        {Situation::CO, "CO"},
        {Situation::DE, "DE"},
        {Situation::DE_CO, "DE_CO"},
        {Situation::SB, "SB"},
        {Situation::SB_CO, "SB_CO"},
        {Situation::SB_DE, "SB_DE"},
        {Situation::SB_CO_DE, "SB_CO_DE"},
        {Situation::BB_CO, "BB_CO"},
        {Situation::BB_DE, "BB_DE"},
        {Situation::BB_SB, "BB_SB"},
        {Situation::BB_CO_DE, "BB_CO_DE"},
        {Situation::BB_CO_SB, "BB_CO_SB"},
        {Situation::BB_DE_SB, "BB_DE_SB"},
        {Situation::BB_CO_DE_SB, "BB_CO_DE_SB"}
};

const map<int, string> card_val_str = {
        {2, "2"},
        {3, "3"},
        {4, "4"},
        {5, "5"},
        {6, "6"},
        {7, "7"},
        {8, "8"},
        {9, "9"},
        {10, "T"},
        {11, "J"},
        {12, "Q"},
        {13, "K"},
        {14, "A"}
};

//const map<Suit, string> card_suit_str = {
//        {Heart, "h"},
//        {Diamond, "d"},
//        {Spade, "s"},
//        {Club, "c"}
//};

const map<Suit, string> card_suit_str = {
        {Heart, "♥"},
        {Diamond, "♦"},
        {Spade, "♠"},
        {Club, "♣"}
};


#endif //POKER_SIMULATOR_ENUMS_H
