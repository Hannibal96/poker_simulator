//
// Created by Neria on 27/06/2020.
//

#include "PokerPlayer.h"


PokerPlayer::PokerPlayer(string &name, int id, Strategy& strategy, Position initial_position)
:name_(name), id_(id), money_(0.0), curr_reward_(0.0), curr_position(initial_position),
last_action(NotAct)
{
    strategy_ = strategy;
}

void PokerPlayer::AddHoldingCards(Card card_a) {
    assert(holding_cards.size() < 2 && "-ASSET- too many holdings cards");
    holding_cards.push_back(card_a);
}

void PokerPlayer::MockHand() {
    assert(holding_cards.size() <= 2 && "-ASSET- too many holdings cards");
    holding_cards.clear();
}


uint16_t calc_hand_hash(uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4, uint8_t c5, uint8_t p1, uint8_t p2 ){
    auto key_p = card[c1] + card[c2] + card[c3] + card[c4] + card[c5] + card[p1] + card[p2];

    int_fast8_t is_suit = flush_check[key_p >> FLUSH_BIT_SHIFT];

    if (NOT_A_SUIT != is_suit) {
        auto * const s = suit_kronecker[is_suit];
        return flush_ranks[s[c1] | s[c2] | s[c3] | s[c4] | s[c5] | s[p1] | s[p2]];
    }
    auto const hash = FACE_BIT_MASK & (uint32_t) (31 * (uint64_t) key_p);
    return rank_hash[offsets[hash >> RANK_OFFSET_SHIFT] + (hash & RANK_HASH_MOD)];
}


uint32_t PokerPlayer::EvaluateHandHash(vector<Card> community_cards) {

    map<Suit, uint8_t> suit_converter = {{Heart, 1}, {Diamond, 2}, {Spade, 0},{Club, 3}};

    uint8_t c0 = uint8_t(14 - community_cards[0].GetValue()) * 4 + suit_converter[community_cards[0].GetSuit()];
    uint8_t c1 = uint8_t(14 - community_cards[1].GetValue()) * 4 + suit_converter[community_cards[1].GetSuit()];
    uint8_t c2 = uint8_t(14 - community_cards[2].GetValue()) * 4 + suit_converter[community_cards[2].GetSuit()];
    uint8_t c3 = uint8_t(14 - community_cards[3].GetValue()) * 4 + suit_converter[community_cards[3].GetSuit()];
    uint8_t c4 = uint8_t(14 - community_cards[4].GetValue()) * 4 + suit_converter[community_cards[4].GetSuit()];
    uint8_t c5 = uint8_t(14 - holding_cards[0].GetValue()) * 4 + suit_converter[holding_cards[0].GetSuit()];
    uint8_t c6 = uint8_t(14 - holding_cards[1].GetValue()) * 4 + suit_converter[holding_cards[1].GetSuit()];


    //chrono::steady_clock sc;   // create an object of `steady_clock` class
    //auto start = sc.now();     // start timer

    best_hand_hash = calc_hand_hash(c0, c1, c2, c3, c4, c5, c6);

    //auto end = sc.now();       // end timer (starting & ending is done by measuring the time at the moment the process started & ended respectively)
    //auto time_span = static_cast<chrono::duration<double>>(end - start);   // measure time span between start & end
    //cout<<"Operation took: "<<time_span.count()<<" seconds !!!";

    return best_hand_hash;
}


uint32_t PokerPlayer::GetPlayerBestHashHand() const {
    return best_hand_hash;
}





Action PokerPlayer::GetAction(History history) {

    assert(last_action == NotAct && "-ASSERT- set action and entering get action");
    if(curr_position == BigBlind && history == BigBlind_In) {
        last_action = AllIn;
        last_situation = CO;
        return AllIn;
    }

    /*
    last_action = vector_strategy.GetAction(holding_cards[0], holding_cards[1]);
    return last_action;*/

    //int hand_num = CalcHand(holding_cards[0], holding_cards[1]);

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
    //State state{situation, hand_num};

    //last_action = bandit_table.get_action(last_situation, holding_cards[0], holding_cards[1]);
    last_action = strategy_.GetAction(last_situation, holding_cards[0], holding_cards[1]);
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
    return strategy_.GetName();
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


Card PokerPlayer::GetHoldingCard1(){
    return holding_cards[0];
}

Card PokerPlayer::GetHoldingCard2(){
    return holding_cards[1];
}

void PokerPlayer::UpdateTable() {
    if(curr_position == BigBlind && last_situation == CO){
        return;
    }

    //int hand_num = CalcHand(holding_cards[0], holding_cards[1]);
    // bandit_table.update_table(last_situation, holding_cards[0], holding_cards[1], last_action, curr_reward_);
    strategy_.UpdateParameters(last_situation, holding_cards[0], holding_cards[1], last_action, curr_reward_);
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

    //player_string += ", \nBest Hand: " + best_hand.ToString();
    player_string += ", \nBest Hand: " + to_string(best_hand_hash);
    player_string += ", Money: "+to_string(money_);
    player_string += ", Reward: "+to_string(curr_reward_);

    player_string += strategy_.get_bandit().ToString();

    return "\n"+player_string+"\n";
}

std::ostream& operator<<(std::ostream& os, PokerPlayer& player){
    os << player.ToString();
    return os;
}



