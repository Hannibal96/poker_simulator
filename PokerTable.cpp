//
// Created by Neria on 28/06/2020.
//

#include "PokerTable.h"
#include <bits/stdc++.h>


PokerTable::PokerTable(PokerPlayer plater_a, PokerPlayer plater_b, PokerPlayer plater_c, PokerPlayer plater_d,
                       double big_blind, double small_blind, double all_in, int table_id) {

    players.push_back(plater_a);
    players.push_back(plater_b);
    players.push_back(plater_c);
    players.push_back(plater_d);

    big_blind_ = big_blind;
    small_blind_ = small_blind;
    all_in_ = all_in;
    table_id_ = table_id;

    curr_pot = 0;
    curr_co_idx = 0;
    hands_counter = 0;
    show_down_counter = 0;
}

void PokerTable::Round() {
    EndRound();

    Deck deck = Deck();
    deck.Shuffle();

    for(int i=0 ; i<TABLE_SIZE ; i++){    // posts blinds
        if(players[i].GetPosition() == SmallBlind){
            players[i].UpdateMoney(-small_blind_);
            curr_pot += small_blind_;
        }
        else if (players[i].GetPosition() == BigBlind){
            players[i].UpdateMoney(-big_blind_);
            curr_pot += big_blind_;
        }
    }

    for(int i=0 ; i<TABLE_SIZE*2 ; i++){    // deal cards
        players[(curr_co_idx+2+i)%TABLE_SIZE].AddHoldingCards(deck.DealCard()); // starts deal from SB
    }

    for(int i=0 ; i<TABLE_SIZE ; i++){      // playing
        Action action = players[(curr_co_idx+i)%TABLE_SIZE].GetAction(previous_action); // starts plays from CO
        if(action == AllIn){
            previous_action = static_cast<PreviousAction >(previous_action+1);
            if(players[(curr_co_idx+i)%TABLE_SIZE].GetPosition() == CutOff || players[(curr_co_idx+i)%TABLE_SIZE].GetPosition() == Dealer){
                players[(curr_co_idx+i)%TABLE_SIZE].UpdateMoney(-all_in_);
                curr_pot += all_in_;
            }
            else if(players[(curr_co_idx+i)%TABLE_SIZE].GetPosition() == SmallBlind){
                players[(curr_co_idx+i)%TABLE_SIZE].UpdateMoney(-all_in_ + small_blind_);
                curr_pot += (+all_in_ - small_blind_);
            }
            else if(players[(curr_co_idx+i)%TABLE_SIZE].GetPosition() == BigBlind){
                players[(curr_co_idx+i)%TABLE_SIZE].UpdateMoney(-all_in_ + big_blind_);
                curr_pot += (+all_in_ - big_blind_);
            }
        }
    }

    // deal community cards
    deck.DealCard();
    community_cards.push_back(deck.DealCard());
    community_cards.push_back(deck.DealCard());
    community_cards.push_back(deck.DealCard());
    deck.DealCard();
    community_cards.push_back(deck.DealCard());
    deck.DealCard();
    community_cards.push_back(deck.DealCard());

    for(int i=0 ; i<TABLE_SIZE ; i++) {      // evaluate hands
        players[i].EvaluateHand(community_cards);
    }

    sort(players.begin(), players.end(), greater<>());
    int winning_counter = 0;
    for(int i=0; i<TABLE_SIZE; i++){
        if(players[0] == players[i])
            winning_counter ++;
    }
    double wins_money = (double)(curr_pot)/winning_counter;
    for(int i=0; i<winning_counter; i++){
        players[i].UpdateMoney(wins_money);
    }

    hands_counter++;
    double total_money = 0;
    for(auto x:players){
        total_money += x.GetMoney();
    }
    assert(abs(total_money) <= 0.1 && "-ASSERT- none zero sum of money, ");

}

void PokerTable::EndRound() {
    curr_pot = 0;
    curr_co_idx = (curr_co_idx + 1) % TABLE_SIZE;
    for(int i=0;i<TABLE_SIZE;i++){
        players[i].UpdatePosition();
        players[i].MockHand();
        players[i].UnSetAction();
    }
    previous_action = Empty;
    community_cards.clear();
}


string PokerTable::ToString() const{        //TODO: change printing format to something more nice
    string table_string = "=====================================================================\n";
    table_string += "========= Table: "+to_string(table_id_)+", #Hand: "+to_string(hands_counter)+"\n\n| ";

    for(auto x: community_cards){
        table_string += x.ToString()+" | ";
    }
    table_string += "\n";

    for(int i=0; i<TABLE_SIZE; i++){
        table_string += players[i].ToString();
    }
    table_string += "\n=====================================================================\n";
    return table_string+"\n";

}

std::ostream& operator<<(std::ostream& os, const PokerTable& table){
    os << table.ToString();
    return os;
}
