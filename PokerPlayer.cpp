//
// Created by Neria on 27/06/2020.
//

#include "PokerPlayer.h"


PokerPlayer::PokerPlayer(string &name, int id, Strategy& strategy)
:name_(name), strategy_(strategy), id_(id), money_(0)
{
}

void PokerPlayer::AddHoldingCards(Card card_a) {
    holding_cards.push_back(card_a);
}

void PokerPlayer::AddHoldingCards(Card card_a, Card card_b) {
    holding_cards.push_back(card_a);
    holding_cards.push_back(card_b);
}

void PokerPlayer::EvaluateHand(vector<Card> community_cards) {
    vector<Card> all_cards = holding_cards;
    all_cards.insert(all_cards.end(), community_cards.begin(), community_cards.end());
    best_hand = GetBestHand(all_cards);
}


