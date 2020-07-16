//
// Created by Neria on 27/06/2020.
//
#include <bits/stdc++.h>
#include "PokerHand.h"

PokerHand::PokerHand(Card card_a, Card card_b, Card card_c, Card card_d, Card card_e) {
    rank = UnSet;
    hand_cards.push_back(card_a);
    hand_cards.push_back(card_b);
    hand_cards.push_back(card_c);
    hand_cards.push_back(card_d);
    hand_cards.push_back(card_e);
    sort(hand_cards.begin(), hand_cards.end(), greater<>());
    this->SetRank();
}

PokerHand::PokerHand() {    // just for having deafault constructor to be able to init Pokerplayer
    rank = UnSet;
}

PokerHand::PokerHand(vector<Card>& cards) {
    rank = UnSet;
    for(auto card:cards){
        hand_cards.push_back(card);
    }
    sort(hand_cards.begin(), hand_cards.end(), greater<>());
    this->SetRank();
}

void PokerHand::SetRank() {
    bool flush_flag = false;

    // flush is charectries by all the kickers value
    //TODO: check why concaturating == not working well
    if(hand_cards[0].GetSuit() == hand_cards[1].GetSuit() && hand_cards[0].GetSuit() == hand_cards[2].GetSuit()
        && hand_cards[0].GetSuit() == hand_cards[3].GetSuit() && hand_cards[0].GetSuit() == hand_cards[4].GetSuit()) {
        assert(rank == UnSet && "-ASSERT- double rank assignment");

        flush_flag = true;
        characteristic.push_back(hand_cards[0].GetValue());
        characteristic.push_back(hand_cards[1].GetValue());
        characteristic.push_back(hand_cards[2].GetValue());
        characteristic.push_back(hand_cards[3].GetValue());
        characteristic.push_back(hand_cards[4].GetValue());
        rank = Flush;
    }

    // stragiht is charectries by the last card in the sequence
    // general stragiht
    if(hand_cards[0].GetValue() == hand_cards[1].GetValue()+1 && hand_cards[0].GetValue() == hand_cards[2].GetValue()+2
        && hand_cards[0].GetValue() == hand_cards[3].GetValue()+3 && hand_cards[0].GetValue() == hand_cards[4].GetValue()+4) {
        if(flush_flag){
            characteristic = vector<Value>();
            rank = StraightFlush;
        } else {
            characteristic.push_back(hand_cards[0].GetValue());
            rank = Straight;
        }
        return;
    }
    // wheel stragiht A-2-3-4-5 <=> A-5-4-3-2
    else if(hand_cards[1].GetValue()+1 == hand_cards[2].GetValue()+2 && hand_cards[1].GetValue()+1 == hand_cards[3].GetValue()+3 &&
            hand_cards[1].GetValue()+1 == hand_cards[4].GetValue()+4 && hand_cards[0].GetValue() == Ace && hand_cards[1].GetValue() == Five) {
        if(flush_flag){
            characteristic = vector<Value>();
            rank = StraightFlush;
        } else {
            characteristic.push_back(hand_cards[1].GetValue());
            rank = Straight;
        }
        return;
    }
    if(flush_flag)
        return;

    // quads are charectries by quads value and kicker
    if(hand_cards[0].GetValue() == hand_cards[1].GetValue() && hand_cards[0].GetValue() == hand_cards[2].GetValue()
            && hand_cards[0].GetValue() == hand_cards[3].GetValue()){
        assert(rank == UnSet && "-ASSERT- double rank assignment");
        rank = Quads;
        characteristic.push_back(hand_cards[0].GetValue());
        characteristic.push_back(hand_cards[4].GetValue());
    }

    else if(hand_cards[1].GetValue() == hand_cards[2].GetValue() && hand_cards[1].GetValue() == hand_cards[3].GetValue()
            && hand_cards[1].GetValue() == hand_cards[4].GetValue()) {
        assert(rank == UnSet && "-ASSERT- double rank assignment");
        rank = Quads;
        characteristic.push_back(hand_cards[1].GetValue());
        characteristic.push_back(hand_cards[0].GetValue());
    }

    // full houses are charectries by tripes value and the pair value
    else if( (hand_cards[0].GetValue() == hand_cards[1].GetValue() && hand_cards[0].GetValue() == hand_cards[2].GetValue())
            && (hand_cards[3].GetValue() == hand_cards[4].GetValue())) {
        assert(rank == UnSet && "-ASSERT- double rank assignment");
        rank = FullHouse;
        characteristic.push_back(hand_cards[0].GetValue());
        characteristic.push_back(hand_cards[3].GetValue());
    }

    else if(hand_cards[0].GetValue() == hand_cards[1].GetValue() &&
            hand_cards[2].GetValue() == hand_cards[3].GetValue() && hand_cards[2].GetValue() == hand_cards[4].GetValue()) {
        assert(rank == UnSet && "-ASSERT- double rank assignment");
        rank = FullHouse;
        characteristic.push_back(hand_cards[0].GetValue());
        characteristic.push_back(hand_cards[2].GetValue());
    }

    // trips are charectries by tripes value and two kickers
    else if(hand_cards[0].GetValue() == hand_cards[1].GetValue() && hand_cards[0].GetValue() == hand_cards[2].GetValue()) {
        assert(rank == UnSet && "-ASSERT- double rank assignment");
        rank = Trips;
        characteristic.push_back(hand_cards[0].GetValue());
        characteristic.push_back(hand_cards[3].GetValue());
        characteristic.push_back(hand_cards[4].GetValue());
    }

    else if(hand_cards[1].GetValue() == hand_cards[2].GetValue() && hand_cards[1].GetValue() == hand_cards[3].GetValue()) {
        assert(rank == UnSet && "-ASSERT- double rank assignment");
        rank = Trips;
        characteristic.push_back(hand_cards[1].GetValue());
        characteristic.push_back(hand_cards[0].GetValue());
        characteristic.push_back(hand_cards[4].GetValue());
    }

    else if(hand_cards[2].GetValue() == hand_cards[3].GetValue() && hand_cards[2].GetValue() == hand_cards[4].GetValue()) {
        assert(rank == UnSet && "-ASSERT- double rank assignment");
        rank = Trips;
        characteristic.push_back(hand_cards[2].GetValue());
        characteristic.push_back(hand_cards[0].GetValue());
        characteristic.push_back(hand_cards[1].GetValue());
    }

    // two pairs are charectries by high pair value and low pair value and one kicker
    // AABBC
    else if(hand_cards[0].GetValue() == hand_cards[1].GetValue() && hand_cards[2].GetValue() == hand_cards[3].GetValue()) {
        assert(rank == UnSet && "-ASSERT- double rank assignment");
        rank = TwoPairs;
        characteristic.push_back(hand_cards[0].GetValue());
        characteristic.push_back(hand_cards[2].GetValue());
        characteristic.push_back(hand_cards[4].GetValue());
    }
    //AABCC
    else if(hand_cards[0].GetValue() == hand_cards[1].GetValue() && hand_cards[3].GetValue() == hand_cards[4].GetValue()) {
        assert(rank == UnSet && "-ASSERT- double rank assignment");
        rank = TwoPairs;
        characteristic.push_back(hand_cards[0].GetValue());
        characteristic.push_back(hand_cards[3].GetValue());
        characteristic.push_back(hand_cards[2].GetValue());
    }
    //ABBCC
    else if(hand_cards[1].GetValue() == hand_cards[2].GetValue() && hand_cards[3].GetValue() == hand_cards[4].GetValue()) {
        assert(rank == UnSet && "-ASSERT- double rank assignment");
        rank = TwoPairs;
        characteristic.push_back(hand_cards[1].GetValue());
        characteristic.push_back(hand_cards[3].GetValue());
        characteristic.push_back(hand_cards[0].GetValue());
    }

    // one pair is charectries by pair value and three kickers
    else if(hand_cards[0].GetValue() == hand_cards[1].GetValue()) {
        assert(rank == UnSet && "-ASSERT- double rank assignment");
        rank = Pair;
        characteristic.push_back(hand_cards[0].GetValue());
        characteristic.push_back(hand_cards[2].GetValue());
        characteristic.push_back(hand_cards[3].GetValue());
        characteristic.push_back(hand_cards[4].GetValue());
    }
    else if(hand_cards[1].GetValue() == hand_cards[2].GetValue()) {
        assert(rank == UnSet && "-ASSERT- double rank assignment");
        rank = Pair;
        characteristic.push_back(hand_cards[1].GetValue());
        characteristic.push_back(hand_cards[0].GetValue());
        characteristic.push_back(hand_cards[3].GetValue());
        characteristic.push_back(hand_cards[4].GetValue());
    }
    else if(hand_cards[2].GetValue() == hand_cards[3].GetValue()) {
        assert(rank == UnSet && "-ASSERT- double rank assignment");
        rank = Pair;
        characteristic.push_back(hand_cards[2].GetValue());
        characteristic.push_back(hand_cards[0].GetValue());
        characteristic.push_back(hand_cards[1].GetValue());
        characteristic.push_back(hand_cards[4].GetValue());
    }
    else if(hand_cards[3].GetValue() == hand_cards[4].GetValue()) {
        assert(rank == UnSet && "-ASSERT- double rank assignment");
        rank = Pair;
        characteristic.push_back(hand_cards[3].GetValue());
        characteristic.push_back(hand_cards[0].GetValue());
        characteristic.push_back(hand_cards[1].GetValue());
        characteristic.push_back(hand_cards[2].GetValue());
    }

    // only high card is left, charecteries by all the cards as kickers
    else{
        assert(rank == UnSet && "-ASSERT- double rank assignment");
        rank = HighCArd;
        characteristic.push_back(hand_cards[0].GetValue());
        characteristic.push_back(hand_cards[1].GetValue());
        characteristic.push_back(hand_cards[2].GetValue());
        characteristic.push_back(hand_cards[3].GetValue());
        characteristic.push_back(hand_cards[4].GetValue());
    }
}

HandRank PokerHand::GetRank(){
    assert(rank != UnSet);
    return rank;
}

bool PokerHand::operator>(const PokerHand& hand){
    assert(hand.rank != UnSet && this->rank != UnSet && "-ASSERT- tried to asses hand before assignment of rank");
    if(this->rank > hand.rank)
        return true;
    if(this->rank < hand.rank || *this == hand)
        return false;
    // hand rank is equal need to compare characteristics
    for(unsigned int idx = 0; idx < characteristic.size() ; idx ++){
        if(characteristic[idx] > hand.characteristic[idx])
            return true;
        if(characteristic[idx] < hand.characteristic[idx])
            return false;
    }
    assert(false && "-ASSERT- reaching here meaning there is problem with == operator of PokerHand");
    return false;
}

bool PokerHand::operator==(const PokerHand& hand){
    assert(hand.rank != UnSet && this->rank != UnSet && "-ASSERT- tried to asses hand before assignment of rank");
    if (hand.rank != this->rank){
        return false;
    }
    for(unsigned int idx = 0; idx < characteristic.size() ; idx ++){
        if(characteristic[idx] != hand.characteristic[idx])
            return false;
    }
    return true;
}

bool PokerHand::operator<(const PokerHand& hand){
    return !(*this == hand || *this > hand);
}

string PokerHand::ToString() const{
    string hand_string = "| ";
    for(auto card: hand_cards){
        hand_string += card.ToString() + " | ";
    }

    switch(rank){
        case HighCArd:
            hand_string += "= HighCard of: " + to_string(characteristic[0]);
            break;
        case Pair:
            hand_string += "= Pair of: " + to_string(characteristic[0]);
            break;
        case TwoPairs:
            hand_string += "= TwoPairs of: " + to_string(characteristic[0]) + " and "+ to_string(characteristic[1]);
            break;
        case Trips:
            hand_string += "= Trips of: " + to_string(characteristic[0]);
            break;
        case Straight:
            hand_string += "= Straight of: " + to_string(characteristic[0]) + " high";
            break;
        case Flush:
            hand_string += "= Flush of: " + to_string(characteristic[0]) + " high";
            break;
        case FullHouse:
            hand_string += "= FullHouse of: " + to_string(characteristic[0]) + ", full of: " + to_string(characteristic[1]);
            break;
        case Quads:
            hand_string += "= Quads of: " + to_string(characteristic[0]);
            break;
        case StraightFlush:
            hand_string += "= StraightFlush of: " + to_string(characteristic[0]) + " high";
            break;
        case UnSet:
            assert(false && "-ASSERT- tries to print hand before it was assign");
            break;
    }
    return hand_string;
}

std::ostream& operator<<(std::ostream& os, const PokerHand& hand){
    os << hand.ToString();
    return os;
}

PokerHand GetBestHand(vector<Card> & seven_cards){     // TODO: overload for 7 cards apart and 2 pvt cards + 5 comm card
    assert(seven_cards.size() == 7);                    // TODO: switch with something less arabic
    PokerHand hand = PokerHand(seven_cards[0], seven_cards[1], seven_cards[2], seven_cards[3], seven_cards[4]);
    PokerHand max_hand = hand;

    hand = PokerHand(seven_cards[0], seven_cards[1], seven_cards[2], seven_cards[3], seven_cards[5]);
    if(hand > max_hand)        max_hand = hand;

    hand = PokerHand(seven_cards[0], seven_cards[1], seven_cards[2], seven_cards[3], seven_cards[6]);
    if(hand > max_hand)        max_hand = hand;

    hand = PokerHand(seven_cards[0], seven_cards[1], seven_cards[2], seven_cards[4], seven_cards[5]);
    if(hand > max_hand)        max_hand = hand;

    hand = PokerHand(seven_cards[0], seven_cards[1], seven_cards[2], seven_cards[4], seven_cards[6]);
    if(hand > max_hand)        max_hand = hand;

    hand = PokerHand(seven_cards[0], seven_cards[1], seven_cards[2], seven_cards[5], seven_cards[6]);
    if(hand > max_hand)        max_hand = hand;

    hand = PokerHand(seven_cards[0], seven_cards[1], seven_cards[3], seven_cards[4], seven_cards[5]);
    if(hand > max_hand)        max_hand = hand;

    hand = PokerHand(seven_cards[0], seven_cards[1], seven_cards[3], seven_cards[4], seven_cards[6]);
    if(hand > max_hand)        max_hand = hand;

    hand = PokerHand(seven_cards[0], seven_cards[1], seven_cards[3], seven_cards[5], seven_cards[6]);
    if(hand > max_hand)        max_hand = hand;

    hand = PokerHand(seven_cards[0], seven_cards[1], seven_cards[4], seven_cards[5], seven_cards[6]);
    if(hand > max_hand)        max_hand = hand;

    hand = PokerHand(seven_cards[0], seven_cards[2], seven_cards[3], seven_cards[4], seven_cards[5]);
    if(hand > max_hand)        max_hand = hand;

    hand = PokerHand(seven_cards[0], seven_cards[2], seven_cards[3], seven_cards[4], seven_cards[6]);
    if(hand > max_hand)        max_hand = hand;

    hand = PokerHand(seven_cards[0], seven_cards[2], seven_cards[3], seven_cards[5], seven_cards[6]);
    if(hand > max_hand)        max_hand = hand;

    hand = PokerHand(seven_cards[0], seven_cards[2], seven_cards[4], seven_cards[5], seven_cards[6]);
    if(hand > max_hand)        max_hand = hand;

    hand = PokerHand(seven_cards[0], seven_cards[3], seven_cards[4], seven_cards[5], seven_cards[6]);
    if(hand > max_hand)        max_hand = hand;

    hand = PokerHand(seven_cards[1], seven_cards[2], seven_cards[3], seven_cards[4], seven_cards[5]);
    if(hand > max_hand)        max_hand = hand;

    hand = PokerHand(seven_cards[1], seven_cards[2], seven_cards[3], seven_cards[4], seven_cards[6]);
    if(hand > max_hand)        max_hand = hand;

    hand = PokerHand(seven_cards[1], seven_cards[2], seven_cards[3], seven_cards[5], seven_cards[6]);
    if(hand > max_hand)        max_hand = hand;

    hand = PokerHand(seven_cards[1], seven_cards[2], seven_cards[4], seven_cards[5], seven_cards[6]);
    if(hand > max_hand)        max_hand = hand;

    hand = PokerHand(seven_cards[1], seven_cards[3], seven_cards[4], seven_cards[5], seven_cards[6]);
    if(hand > max_hand)        max_hand = hand;

    hand = PokerHand(seven_cards[2], seven_cards[3], seven_cards[4], seven_cards[5], seven_cards[6]);
    if(hand > max_hand)        max_hand = hand;

    return max_hand;

}


