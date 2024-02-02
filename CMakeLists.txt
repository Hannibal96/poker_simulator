cmake_minimum_required(VERSION 3.15)
project(poker_simulator)

set(CMAKE_CXX_STANDARD 14)

add_executable(poker_simulator main.cpp deck_utils/Card.cpp deck_utils/Card.h deck_utils/Deck.cpp deck_utils/Deck.h agents_utils/PokerPlayer.cpp agents_utils/PokerPlayer.h poker_utils/PokerTable.cpp poker_utils/PokerTable.h poker_utils/Enums.h agents_utils/Agent.h agents_utils/Bandit_Agent.cpp agents_utils/Bandit_Agent.h agents_utils/Epsilon_Greedy_Agent.cpp agents_utils/Epsilon_Greedy_Agent.h agents_utils/MWU_Agent.cpp agents_utils/MWU_Agent.h agents_utils/Fix_Strategy_Agent.cpp agents_utils/Fix_Strategy_Agent.h jsons_utils/json.cpp jsons_utils/json.h agents_utils/Agent.cpp poker_utils/Enums.cpp)