//
// Created by neria on 12/01/2024.
//

#include "json.h"

Agent* ParseAgent(const nlohmann::json& agentJson){
    string agentName = agentJson["Name"];
    string agentType = agentJson["Algorithm"];
    if(agentType == "Bandit"){
        return new Bandit_Agent(agentName);
    }
    if(agentType == "MWU"){
        auto params = agentJson["Parameters"];
        double beta = params["Beta"];
        unsigned int T = params["T"];
        double init_p = params["InitP"];
        return new MWU_Agent(agentName, beta, T, init_p);
    }
    if(agentType == "EpsilonGreedy"){
        auto params = agentJson["Parameters"];
        double init_epsilon = params["InitEpsilon"];
        double min_epsilon = params["MinEpsilon"];
        double decay_rate = params["DecayRate"];
        return new Epsilon_Greedy_Agent(agentName, init_epsilon, min_epsilon, decay_rate);
    }
    if(agentType == "FixStrategy"){
        // TODO: implement
        throw;
    }
    return nullptr;
}


vector<PokerPlayer> parseJsonFilePlayers(const std::string& fileName){
    std::ifstream file(fileName);
    nlohmann::json jsonData;
    file >> jsonData;
    file.close();

    vector<PokerPlayer> players;
    for(int i = 0; i < 4; i++){
        string player_string = "Player" + to_string(i+1);
        Agent* agent = ParseAgent(jsonData["Players"][player_string]);
        string name = jsonData["Players"][player_string]["Name"];
        Position pos = static_cast<Position>(pos_map.at(jsonData["Players"][player_string]["InitPosition"]));
        if(agent != nullptr){
            PokerPlayer player = PokerPlayer(name, *agent, pos);
            players.push_back(player);
        }
        else{
            cout << "Error: agent is nullptr" << endl;
            exit(1);
        }
    }

    return players;
}


map<string, double> parseJsonFileTable(const std::string& fileName){
    std::ifstream file(fileName);
    nlohmann::json jsonData;
    file >> jsonData;
    file.close();

    double all_in = jsonData["Table"]["AllIn"];
    double big_blind = jsonData["Table"]["BigBlind"];
    double small_blind = jsonData["Table"]["SmallBlind"];
    double jackpot = jsonData["Table"]["JackPot"];
    double factor = jsonData["Table"]["Factor"];

    return map<string, double>{{"all_in", all_in * factor},
                               {"big_blind", big_blind * factor},
                               {"small_blind", small_blind * factor},
                               {"jack_pot", jackpot * factor}    };
}

string parseJsonFileOutput(const std::string& fileName){
    std::ifstream file(fileName);
    nlohmann::json jsonData;
    file >> jsonData;
    file.close();
    auto env_map = jsonData["Simulation"];
    string output_path = env_map["OutPut"];
    return output_path;
}

map<string, double> parseJsonFileEnv(const std::string& fileName){
    std::ifstream file(fileName);
    nlohmann::json jsonData;
    file >> jsonData;
    file.close();

    auto env_map = jsonData["Simulation"];
    int seed = env_map["Seed"];
    int repeats = env_map["Repeats"];
    int rounds = env_map["Rounds"];
    bool update_pos = env_map["UpdatePositions"];

    return map<string, double>{{"seed", seed},
                               {"repeats", repeats},
                               {"rounds", rounds},
                                {"print", env_map["Print"]},
                               {"update_pos", update_pos}
                               };
}
