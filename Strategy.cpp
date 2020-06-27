//
// Created by Neria on 27/06/2020.
//

#include "Strategy.h"
#include <iostream>

Strategy::Strategy(vector<int>& unsuited_strategy, vector<int>& suited_strategy, string& name):
     unsuited_strategy_ (unsuited_strategy), suited_strategy_(suited_strategy), name_(name)
        {}

Action Strategy::GetAction(Card card_a, Card card_b) {
    Value max_val = (card_a.GetValue() > card_b.GetValue()) ? card_a.GetValue() : card_b.GetValue();
    Value min_val = (card_a.GetValue() <= card_b.GetValue()) ? card_a.GetValue() : card_b.GetValue();

    vector<int> strategy = (card_a.GetSuit() == card_b.GetSuit()) ? suited_strategy_ : unsuited_strategy_;

    int sum = 0;
    for(int idx=2 ; idx <=max_val ; idx++){
        sum += strategy[idx-2];
    }
    return (max_val-min_val < sum) ? AllIn : Fold ;
}