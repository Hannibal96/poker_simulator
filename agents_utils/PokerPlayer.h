//
// Created by Neria on 27/06/2020.
//

#ifndef POKER_SIMULATOR_POKERPLAYER_H
#define POKER_SIMULATOR_POKERPLAYER_H

#include "../poker_utils/Enums.h"
#include <bits/stdc++.h>
#include <iostream>
#include "../poker_utils/hashes.h"
#include "../agents_utils/Agent.h"

class PokerPlayer {
private:
    string name_;
    double curr_reward_;
    long double money_;

    vector<Card> holding_cards;
    uint32_t best_hand_hash;
    Position curr_position;

    Action last_action;
    Situation last_situation;

    Agent &agent;

    static uint8_t calc_card_hash(Card c){
        return uint8_t(14 - c.GetValue()) * 4 + hash_suit_converter.at(c.GetSuit());
    }
    static uint16_t calc_hand_hash(uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4, uint8_t c5, uint8_t p1, uint8_t p2 ){
        auto key_p = card[c1] + card[c2] + card[c3] + card[c4] + card[c5] + card[p1] + card[p2];

        int_fast8_t is_suit = flush_check[key_p >> FLUSH_BIT_SHIFT];

        if (NOT_A_SUIT != is_suit) {
            auto * const s = suit_kronecker[is_suit];
            return flush_ranks[s[c1] | s[c2] | s[c3] | s[c4] | s[c5] | s[p1] | s[p2]];
        }
        auto const hash = FACE_BIT_MASK & (uint32_t) (31 * (uint64_t) key_p);
        return rank_hash[offsets[hash >> RANK_OFFSET_SHIFT] + (hash & RANK_HASH_MOD)];
    }


public:
    PokerPlayer(string  name, Agent& agent, Position initial_position);
    ~PokerPlayer() = default;
    void AddHoldingCards(Card card_a);
    void MockHand();
    Action GetAction(History history);
    Position GetPosition();

    Card GetHoldingCard1();
    Card GetHoldingCard2();
    int CalcHandIdx();

    long double GetMoney() const;
    uint32_t GetPlayerBestHashHand() const ;
    uint32_t EvaluateHandHash(vector<Card> community_cards);
    bool IsJAckPot(vector<Card> community_cards);
    void UpdatePosition();
    void UpdateMoney(double delta);
    void UnSetAction();
    void ResetReward();
    void UpdateTable();
    Agent &GetAgent(){
        return agent;
    }

    string ToString() ;
    bool operator>(const PokerPlayer& player);
    bool operator==(const PokerPlayer& player);
    bool operator<(const PokerPlayer& player);



};

#endif //POKER_SIMULATOR_POKERPLAYER_H
