cmake_minimum_required(VERSION 3.15)
project(poker_simulator)

set(CMAKE_CXX_STANDARD 14)

add_executable(poker_simulator main.cpp Card.cpp Card.h Deck.cpp Deck.h PokerHand.cpp PokerHand.h Strategy.cpp Strategy.h PokerPlayer.cpp PokerPlayer.h)