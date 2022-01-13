//
// Created by neria on 12/01/2022.
//

#ifndef POKER_SIMULATOR_SLIMDECK_H
#define POKER_SIMULATOR_SLIMDECK_H

using namespace std;

#include <vector>
#include <iostream>

typedef uint8_t SlimCard;

class SlimDeck {
public:
    SlimDeck();
    ~SlimDeck() = default;

    SlimCard DealCard();
    void Shuffle();

    string ToString() const;
    static int curr_seed;

private:
    vector<SlimCard> cards;
};

std::ostream& operator<<(std::ostream& os, const SlimDeck& deck);


#endif //POKER_SIMULATOR_SLIMDECK_H
