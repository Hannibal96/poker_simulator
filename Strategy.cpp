//
// Created by Neria on 27/06/2020.
//

#include "Strategy.h"
#include <iostream>

string Strategy::GetName() {
    return name_;
}

Action Strategy::GetAction() {
    return NotAct;
}

void Strategy::UpdateParameters() {

}