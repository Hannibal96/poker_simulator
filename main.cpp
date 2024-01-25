#include <iostream>
#include <chrono>
#include "deck_utils/Card.h"
#include "agents_utils/PokerPlayer.h"
#include "poker_utils/PokerTable.h"
#include "jsons_utils/json.h"

using namespace std;

void sim_print(PokerTable table, int i);
string parse_args(int argc, char *argv[]);
void printProgressBar(int percentage, double ips);

int main(int argc, char *argv[]) {

    string json_path = parse_args(argc, argv);

    vector<PokerPlayer> players = parseJsonFilePlayers(json_path);
    map<string, double> table_params = parseJsonFileTable(json_path);
    string output_path = parseJsonFileOutput(json_path);
    map<string, double> env_map = parseJsonFileEnv(json_path);

    if(!output_path.empty())
        freopen(output_path.c_str(), "w", stdout);

    double bb = table_params["big_blind"],
           sb = table_params["small_blind"],
           all_in = table_params["all_in"],
           jack_pot = table_params["jack_pot"];

    int print = int(env_map["print"]),
        repeats = int(env_map["repeats"]),
        rounds = int(env_map["rounds"]),
        seed = int(env_map["seed"]); // TODO: use seed

    bool update_pos = bool(env_map["update_pos"]);

    PokerTable table = PokerTable(players,
                                  bb, sb, all_in, jack_pot ,
                                  update_pos, repeats);

    auto startTime = std::chrono::high_resolution_clock::now();

    for(int i=0; i < rounds; i++){
        table.Round();

        if((i+1) % (rounds/100 + not (rounds/100)) == 0 or i == 0){
            auto currentTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsedSeconds = currentTime - startTime;
            double ips = i / elapsedSeconds.count();
            printProgressBar((i+1)*100/rounds, ips);
        }

        if((i+1) % print == 0 || i == 0) {
            sim_print(table, i);
        }
    }

    if(!output_path.empty())
        freopen("/dev/tty", "a", stdout);
    return 0;
}

string parse_args(int argc, char *argv[]){
    string json_path;
    for(int i=1; i < argc; i++){
        if(strcmp(argv[i], "-j") == 0 || strcmp(argv[i], "--json") == 0 || strcmp(argv[i], "-json") == 0){
            json_path = argv[i+1];
        }
        else if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-help") == 0){
            cout << "Usage: " << argv[0] << " [-j/--json/-json] [path to json file]" << endl;
            exit(0);
        }
        else{
            cout << "Invalid argument: " << argv[i] << endl;
            exit(1);
        }
        i++;
    }
    if(json_path.empty()){
        cout << "Missing json file path" << endl;
        exit(1);
    }

    if(json_path.find('/') != string::npos || json_path.find('\\') != string::npos)
        return json_path;
    else
        return "../JSONS/" + json_path;
}


void sim_print(PokerTable table, int i){
    cout << "==============================================================================================================" << endl;
    cout << "============================******************************************************============================" << endl;
    cout << "==============================================================================================================" << endl;

    cout << table << endl;
    cout << table.GetStatsString((i + 1)) << std::endl;
}


void printProgressBar(int percentage, double ips) {
    const int barWidth = 70;

    std::cerr << "[";
    int pos = barWidth * percentage / 100;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cerr << "=";
        else if (i == pos) std::cerr << ">";
        else std::cerr << " ";
    }
    std::cerr << "] " << percentage << "% (" << std::fixed << std::setprecision(2) << ips << " IPS)\r";
    std::cerr.flush();
}