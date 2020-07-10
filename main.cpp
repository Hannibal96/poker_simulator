#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include "Card.h"
#include "Deck.h"
#include "PokerHand.h"
#include "Strategy.h"
#include "PokerPlayer.h"
#include "PokerTable.h"

#define REPEATS 10000000

using namespace std;

map<HandRank, string> SetRanksNames();
map<HandRank, int > SetRanksStats();
void UpdateRanksStats(map<HandRank, int > & ranks_stats, PokerHand hand );

int main() {

    map<HandRank, string> ranks_names = SetRanksNames();
    map<HandRank, int > ranks_stats = SetRanksStats();

    vector<int> stg_a = vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1};
    vector<int> stg_b = vector<int>{1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 8};
    string name = "TestStrategy";
    Strategy stg = Strategy(stg_a, stg_b, name);

    string name1 = "Player1";
    string name2 = "Player2";
    string name3 = "Player3";
    string name4 = "Player4";

    int counter = 0;
    PokerPlayer player1 = PokerPlayer(name1, 11, stg, CutOff);
    PokerPlayer player2 = PokerPlayer(name2, 12, stg, Dealer);
    PokerPlayer player3 = PokerPlayer(name3, 13, stg, SmallBlind);
    PokerPlayer player4 = PokerPlayer(name4, 14, stg, BigBlind);

    PokerTable table = PokerTable(player1, player2, player3, player4, 0.25, 0.1, 2.0 ,10);

    for(int i=0; i<REPEATS; i++){

        table.Round();
        cout << table << endl;

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

