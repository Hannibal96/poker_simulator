#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include "Card.h"
#include "Deck.h"
#include "PokerHand.h"
#include "Strategy.h"

#define REPEATS 10000000

using namespace std;

map<HandRank, string> SetRanksNames();
map<HandRank, int > SetRanksStats();
void UpdateRanksStats(map<HandRank, int > & ranks_stats, PokerHand hand );

int main() {

    vector<int> stg_a = vector<int>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2};
    vector<int> stg_b = vector<int>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    string name = "Test";
    Strategy stg = Strategy(stg_a, stg_b, name);

    int counter = 0;
    for(int i=0; i<REPEATS; i++){
        Deck deck = Deck();
        deck.Shuffle();
        Card card1 = deck.DealCard();
        Card card2 = deck.DealCard();
        if(stg.GetAction(card1, card2) == AllIn){
            counter ++;
        }
        if( (i+1) % (REPEATS/10) == 0){
            cout << 100*(double)counter/(i+1) << endl;
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

