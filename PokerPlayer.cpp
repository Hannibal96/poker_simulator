//
// Created by Neria on 27/06/2020.
//

#include "PokerPlayer.h"


PokerPlayer::PokerPlayer(string &name, int id, VectorStrategy& strategy, Position initial_position)
:name_(name), vector_strategy(strategy), id_(id), money_(0.0), curr_reward_(0.0), curr_position(initial_position),
last_action(NotAct), bandit_table()
{}

void PokerPlayer::AddHoldingCards(Card card_a) {
    assert(holding_cards.size() < 2 && "-ASSET- too many holdings cards");
    holding_cards.push_back(card_a);
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

int CalcHand(Card a, Card b){
    int hand_num;
    if(a.GetSuit() != b.GetSuit()) {
        if (a.GetValue() >= b.GetValue()) {
            hand_num = (a.GetValue() - 2) * 13 + (b.GetValue() - 2);
        } else {
            hand_num = (b.GetValue() - 2) * 13 + (a.GetValue() - 2);
        }
    }
    else{
        if (a.GetValue() >= b.GetValue()) {
            hand_num = (b.GetValue() - 2) * 13 + (a.GetValue() - 2);
        } else {
            hand_num = (a.GetValue() - 2) * 13 + (b.GetValue() - 2);
        }
    }
    return hand_num;
}


Action PokerPlayer::GetAction(History history) {

    assert(last_action == NotAct && "-ASSERT- set action and entering get action");
    if(curr_position == BigBlind && history == BigBlind_In) {
        last_action = AllIn;
        return AllIn;
    }

    /*
    last_action = vector_strategy.GetAction(holding_cards[0], holding_cards[1]);
    return last_action;*/

    int hand_num = CalcHand(holding_cards[0], holding_cards[1]);

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

    else{
        assert(false);
    }

    last_situation = situation;
    State state{situation, hand_num};

    last_action = bandit_table.get_action(state);
    return last_action;
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

double PokerPlayer::GetID() const {
    return id_;
}

string PokerPlayer::GetStrategyName() {
    return vector_strategy.GetName();
}

double PokerPlayer::GetMoney() const{
    return money_;
}

void PokerPlayer::UnSetAction() {
    last_action = NotAct;
}

void PokerPlayer::ResetReward(){
    curr_reward_ = 0.0;
}


void PokerPlayer::UpdateTable() {

    int hand_num = CalcHand(holding_cards[0], holding_cards[1]);
    bandit_table.update_table(last_situation, hand_num, last_action, curr_reward_);
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
}

bool PokerPlayer::operator<(const PokerPlayer &player) {
    return !(*this == player || *this > player);
}


string PokerPlayer::ToString() {
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
    player_string += ", Reward: "+to_string(curr_reward_);

    player_string += bandit_table.ToString();

    return "\n"+player_string+"\n";
}

std::ostream& operator<<(std::ostream& os, PokerPlayer& player){
    os << player.ToString();
    return os;
}



