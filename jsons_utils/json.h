//
// Created by neria on 12/01/2024.
//

#ifndef POKER_SIMULATOR_JSON_H
#define POKER_SIMULATOR_JSON_H
#include <iostream>
#include "../agents_utils/Agent.h"
#include <vector>
#include "json.hpp"
#include "../agents_utils/Bandit_Agent.h"
#include "../agents_utils/Epsilon_Greedy_Agent.h"
#include "../agents_utils/MWU_Agent.h"
#include "../agents_utils/PokerPlayer.h"
#include <fstream>
#include <map>
using namespace std;


string parseJsonFileOutput(const std::string& fileName);
map<string, double> parseJsonFileTable(const std::string& fileName);
vector<PokerPlayer> parseJsonFilePlayers(const std::string& fileName);
Agent* ParseAgent(const nlohmann::json& agentJson);
map<string, double> parseJsonFileEnv(const std::string& fileName);

#endif //POKER_SIMULATOR_JSON_H
