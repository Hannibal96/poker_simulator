//
// Created by Neria on 27/06/2020.
//

#include "PokerPlayer.h"

#include <utility>


PokerPlayer::PokerPlayer(string name, Agent& agent, Position initial_position)
:name_(std::move(name)), money_(0.0), curr_reward_(0.0), curr_position(initial_position),
last_action(NotAct), agent(agent) {}

void PokerPlayer::AddHoldingCards(Card card_a) {
    assert(holding_cards.size() < 2 && "-ASSET- too many holdings cards");
    holding_cards.push_back(card_a);
}

void PokerPlayer::MockHand() {
    assert(holding_cards.size() <= 2 && "-ASSET- too many holdings cards");
    holding_cards.clear();
}

Action PokerPlayer::GetAction(History history) {

    assert(last_action == NotAct && "-ASSERT- set action and entering get action");
    if(curr_position == BigBlind && history == BigBlind_In) {
        last_action = AllIn;
        last_situation = CO;
        return AllIn;
    }

    Situation situation;

    if(curr_position == CutOff){
        situation = CO;
        assert(history == BigBlind_In);
    }

    else if(curr_position == Dealer){
        if(history == BigBlind_In)
            situation = DE;
        else if(history == CutOff_In)
            situation = DE_CO;
        else
            assert(false);
    }

    else if(curr_position == SmallBlind){
        if(history == BigBlind_In)
            situation = SB;
        else if(history == CutOff_In)
            situation = SB_CO;
        else if(history == Dealer_In)
            situation = SB_DE;
        else if(history == CutOff_Dealer_In)
            situation = SB_CO_DE;
        else
            assert(false);
    }

    else if(curr_position == BigBlind){
        if(history == CutOff_In)
            situation = BB_CO;
        else if(history == Dealer_In)
            situation = BB_DE;
        else if(history == SmallBlind_In)
            situation = BB_SB;
        else if(history == CutOff_Dealer_In)
            situation = BB_CO_DE;
        else if(history == CutOff_SmallBlind_In)
            situation = BB_CO_SB;
        else if(history == Dealer_SmallBlind_In)
            situation = BB_DE_SB;
        else if(history == CutOff_Dealer_SmallBlind_In)
            situation = BB_CO_DE_SB;
        else
            assert(false);
    }
    else
        assert(false);

    last_situation = situation;  // TODO: remove asserts after debugging
    State state{last_situation, this->CalcHandIdx()};
    last_action = agent.get_action(state);
    return last_action;
}

void PokerPlayer::UpdateTable() {
    if(curr_position == BigBlind && last_situation == CO){  // TODO: watch last situation
        return;
    }
    State state{last_situation, this->CalcHandIdx()};
    agent.update_parameters(state, last_action, curr_reward_);
}

int PokerPlayer::CalcHandIdx(){
    int M = max(holding_cards[0].GetValue(), holding_cards[1].GetValue()) - 2;
    int m = min(holding_cards[0].GetValue(), holding_cards[1].GetValue()) - 2;
    int suited = int(holding_cards[0].GetSuit() == holding_cards[1].GetSuit());
    return M * M + 2 * m + suited;
}


uint32_t PokerPlayer::EvaluateHandHash(vector<Card> community_cards) {

    uint8_t c0 = calc_card_hash(community_cards[0]);
    uint8_t c1 = calc_card_hash(community_cards[1]);
    uint8_t c2 = calc_card_hash(community_cards[2]);
    uint8_t c3 = calc_card_hash(community_cards[3]);
    uint8_t c4 = calc_card_hash(community_cards[4]);
    uint8_t c5 = calc_card_hash(holding_cards[0]);
    uint8_t c6 = calc_card_hash(holding_cards[1]);

    best_hand_hash = calc_hand_hash(c0, c1, c2, c3, c4, c5, c6);

    return best_hand_hash;
}

bool PokerPlayer::IsJAckPot(vector<Card> community_cards){
    if(this->GetPlayerBestHashHand() < 7453) // Test straight flush
        return false;

    // Test if the player must use the two holding cards for the straight flush
    uint8_t c0 = calc_card_hash(community_cards[0]);
    uint8_t c1 = calc_card_hash(community_cards[1]);
    uint8_t c2 = calc_card_hash(community_cards[2]);
    uint8_t c3 = calc_card_hash(community_cards[3]);
    uint8_t c4 = calc_card_hash(community_cards[4]);

    uint8_t c5 = calc_card_hash(holding_cards[0]);
    uint8_t c6 = calc_card_hash(holding_cards[1]);
    // Dummy cards to test if the player use the both holding cards for the best hand
    uint8_t c7 = uint8_t(14 - holding_cards[0].GetValue()) * 4 + (hash_suit_converter.at(holding_cards[0].GetSuit()) + 1) % 4;
    uint8_t c8 = uint8_t(14 - holding_cards[1].GetValue()) * 4 + (hash_suit_converter.at(holding_cards[1].GetSuit()) + 1) % 4;

    uint32_t change_1 = calc_hand_hash(c0, c1, c2, c3, c4, c5, c8);
    uint32_t change_2 = calc_hand_hash(c0, c1, c2, c3, c4, c7, c6);

    if(change_1 < best_hand_hash && change_2 < best_hand_hash)
        return true;
    return false;
}

uint32_t PokerPlayer::GetPlayerBestHashHand() const {
    return best_hand_hash;
}

void PokerPlayer::UpdateMoney(double delta) {
    money_ += delta;
    curr_reward_ += delta;
}

void PokerPlayer::UpdatePosition() {
    curr_position = Position(((int)curr_position + 1) % 4);
}

Position PokerPlayer::GetPosition() {
    return curr_position;
}

long double PokerPlayer::GetMoney() const{
    return money_;
}

void PokerPlayer::UnSetAction() {
    last_action = NotAct;
}

void PokerPlayer::ResetReward(){
    curr_reward_ = 0.0;
}

Card PokerPlayer::GetHoldingCard1(){
    return holding_cards[0];
}

Card PokerPlayer::GetHoldingCard2(){
    return holding_cards[1];
}

bool PokerPlayer::operator==(const PokerPlayer &player) {
    assert(last_action != NotAct && "-ASSERT- unset last action");
    return this->last_action == player.last_action && this->best_hand_hash == player.best_hand_hash;
}

bool PokerPlayer::operator>(const PokerPlayer &player) {
    assert(last_action != NotAct && "-ASSERT- unset last action");
    if(this == &player)
        return false;
    if(this->last_action == player.last_action)
        return this->best_hand_hash > player.best_hand_hash;
    if(this->last_action == AllIn &&  player.last_action == Fold)
        return true;
    if(this->last_action == Fold &&  player.last_action == AllIn)
        return false;
    assert(false && "-ASSERT- not supposed to reach this part");
    // TODO: remove assert after debugging
    return false;
}

bool PokerPlayer::operator<(const PokerPlayer &player) {
    return !(*this == player || *this > player);
}

string PokerPlayer::ToString() {
    string player_string = "Player ID: ";
    player_string += ", Position: " + positions_names.at(curr_position);
    player_string += ", Holding Cards: | ";
    for(auto x: holding_cards){
        player_string += x.ToString() + " | ";
    }

    player_string += ", Action: " + action_names.at(last_action);

    player_string += ", \nBest Hand: " + to_string(best_hand_hash);
    player_string += ", Money: "+to_string(money_);
    player_string += ", Reward: "+to_string(curr_reward_);
    player_string += "\n";
    player_string += agent.ToString();

    return "\n"+player_string+"\n";
}

std::ostream& operator<<(std::ostream& os, PokerPlayer& player){
    os << player.ToString();
    return os;
}



