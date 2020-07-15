//
// Created by Neria on 27/06/2020.
//

#include "PokerPlayer.h"


PokerPlayer::PokerPlayer(string &name, int id, Strategy& strategy, Position initial_position)
:name_(name), strategy_(strategy), id_(id), money_(0.0), curr_position(initial_position), last_action(NotAct)
{}

void PokerPlayer::AddHoldingCards(Card card_a) {
    assert(holding_cards.size() < 2 && "-ASSET- too many holdings cards");
    holding_cards.push_back(card_a);
}

void PokerPlayer::AddHoldingCards(Card card_a, Card card_b) {
    assert(holding_cards.size() < 2 && "-ASSET- too many holdings cards");
    holding_cards.push_back(card_a);
    holding_cards.push_back(card_b);
}

void PokerPlayer::MockHand() {
    assert(holding_cards.size() <= 2 && "-ASSET- too many holdings cards");
    holding_cards.clear();
}


PokerHand PokerPlayer::EvaluateHand(vector<Card> community_cards) {
    vector<Card> all_cards = holding_cards;
    all_cards.insert(all_cards.end(), community_cards.begin(), community_cards.end());
    best_hand = GetBestHand(all_cards);
    return best_hand;
}

PokerHand PokerPlayer::GetPlayerBestHand() const {
    return best_hand;
}

Action PokerPlayer::GetAction(PreviousAction previous_action) { // TODO: expand for all the other <position, previous_action> combinations
    assert(last_action == NotAct && "-ASSERT- set action and entering get action");
    if(curr_position == BigBlind && previous_action == Empty) {
        last_action = AllIn;
        return AllIn;
    }
    Action action = strategy_.GetAction(holding_cards[0], holding_cards[1]);
    last_action = action;
    return action;
}

Action PokerPlayer::GetAction() const{
    assert(last_action != NotAct && "-ASSERT- unset action");
    return last_action;
}

void PokerPlayer::UpdateMoney(double delta) {
    money_ += delta;
}

void PokerPlayer::UpdatePosition() {
    curr_position = Position(((int)curr_position + 1) % 4);
}

Position PokerPlayer::GetPosition() {
    return curr_position;
}

double PokerPlayer::GetID() const {
    return id_;
}

double PokerPlayer::GetMoney() const{
    return money_;
}

void PokerPlayer::UnSetAction() {
    last_action = NotAct;
}

bool PokerPlayer::operator==(const PokerPlayer &player) {
    assert(last_action != NotAct && "-ASSERT- unset last action");
    return this->last_action == player.last_action && this->best_hand == player.best_hand;
}

bool PokerPlayer::operator>(const PokerPlayer &player) {
    assert(last_action != NotAct && "-ASSERT- unset last action");
    if(this == &player)
        return false;
    if(this->last_action == player.last_action)
        return this->best_hand > player.best_hand;
    if(this->last_action == AllIn &&  player.last_action == Fold)
        return true;
    if(this->last_action == Fold &&  player.last_action == AllIn)
        return false;
    assert(false && "-ASSERT- not supposed to reach this part");
    return true;
}

string PokerPlayer::ToString() const{
    string player_string = "Player ID: "+to_string(id_);
    player_string += ", Position: ";
    switch (curr_position){
        case CutOff:
            player_string += "CutOff";
            break;
        case Dealer:
            player_string += "Dealer";
            break;
        case SmallBlind:
            player_string += "SmallBlind";
            break;
        case BigBlind:
            player_string += "BigBlind";
            break;
    }
    player_string += ", Holding Cards: | ";
    for(auto x: holding_cards){
        player_string += x.ToString() + " | ";
    }

    player_string += ", Action: ";
    switch (last_action){
        case AllIn:
            player_string += "AllIn";
            break;
        case Fold:
            player_string += "Fold";
            break;
    }

    player_string += ", \nBest Hand: "+best_hand.ToString();

    player_string += ", Money: "+to_string(money_);

    return "\n"+player_string+"\n";
}

std::ostream& operator<<(std::ostream& os, const PokerPlayer& player){
    os << player.ToString();
    return os;
}



