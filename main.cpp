#include <iostream>
#include <chrono>
#include "deck_utils/Card.h"
#include "agents_utils/PokerPlayer.h"
#include "poker_utils/PokerTable.h"
#include "jsons_utils/json.h"

using namespace std;

void sim_print(PokerTable table, int i);
map<string, string> parse_args(int argc, char *argv[]);
void printProgressBar(int percentage, double ips);

void print_thread(int i){
    this_thread::sleep_for(chrono::seconds(1));
    cout << "Thread " << i << " is running" << endl;
}

int main(int argc, char *argv[]) {

    map<string, string> arguments = parse_args(argc, argv);
    string json_path = arguments["json"];

    string output_path;
    if(arguments.find("output") != arguments.end()){
        output_path = arguments["output"];
        if(output_path.find('.') == string::npos){
            output_path += ".log";
        }
        output_path = "./../Logs/" + output_path;
    } else {
        output_path = parseJsonFileOutput(json_path);
    }

    vector<PokerPlayer> players = parseJsonFilePlayers(json_path);
    map<string, double> table_params = parseJsonFileTable(json_path);
    map<string, double> env_map = parseJsonFileEnv(json_path);

    int seed;
    if(arguments.find("seed") != arguments.end()){
        seed = stoi(arguments["seed"]);
    } else {
        seed = int(env_map["seed"]);
    }
    global_seed = seed;
    globalGen.seed(seed);

    //if(!output_path.empty())
    //    freopen(output_path.c_str(), "w", stdout);

    double bb = table_params["big_blind"],
           sb = table_params["small_blind"],
           all_in = table_params["all_in"],
           jack_pot = table_params["jack_pot"];

    int print = int(env_map["print"]),
        repeats = int(env_map["repeats"]);

    uint64_t rounds = uint64_t(env_map["rounds"]);
    int numberOfThreads = int(env_map["threads"]);
    bool update_pos = bool(env_map["update_pos"]);
    uint64_t rounds_per_thread = rounds / numberOfThreads;

    auto startTime = std::chrono::high_resolution_clock::now();

//    PokerTable table1 = PokerTable(players, bb, sb, all_in, jack_pot, update_pos, repeats);
//    PokerTable table2 = PokerTable(players, bb, sb, all_in, jack_pot, update_pos, repeats);
//    PokerTable table3 = PokerTable(players, bb, sb, all_in, jack_pot, update_pos, repeats);
//    PokerTable table4 = PokerTable(players, bb, sb, all_in, jack_pot, update_pos, repeats);
//    PokerTable table5 = PokerTable(players, bb, sb, all_in, jack_pot, update_pos, repeats);
//
//    thread t1(&PokerTable::RunRounds, &table1, rounds_per_thread);
//    thread t2(&PokerTable::RunRounds, &table2, rounds_per_thread);
//    thread t3(&PokerTable::RunRounds, &table3, rounds_per_thread);
//    thread t4(&PokerTable::RunRounds, &table4, rounds_per_thread);
//    thread t5(&PokerTable::RunRounds, &table5, rounds_per_thread);

//    t1.join();
//    t2.join();
//    t3.join();
//    t4.join();
//    t5.join();


    vector<std::thread> threads;
    threads.reserve(numberOfThreads);
    vector<PokerTable> tables;
    tables.reserve(numberOfThreads);
    for (int i = 0; i < numberOfThreads; ++i) {
        tables.emplace_back(players, bb, sb,
                            all_in, jack_pot, update_pos, repeats);
        //threads.emplace_back(print_thread, i);
        threads.emplace_back(&PokerTable::RunRounds, &tables[i], rounds_per_thread);
    }
    for (auto& t : threads) {
        t.join();
    }

    // printProgressBar((i+1)*100/rounds, ips);
    // sim_print(table, i);

    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedSeconds = currentTime - startTime;
    double ips = rounds / elapsedSeconds.count();
    cout << ips << endl;

    if(!output_path.empty())
        freopen("/dev/tty", "a", stdout);
    return 0;
}

map<string, string> parse_args(int argc, char *argv[]){
    string json_path, seed, output;
    map<string, string> arguments;
    for(int i=1; i < argc; i++){
        if(strcmp(argv[i], "-j") == 0 || strcmp(argv[i], "--json") == 0 || strcmp(argv[i], "-json") == 0){
            json_path = argv[i+1];
        }
        else if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-help") == 0){
            cout << "Usage: " << argv[0] << " [-j/--json/-json] [path to json file]" << endl;
            exit(0);
        }
        else if(strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--out") == 0 || strcmp(argv[i], "-out") == 0
                    || strcmp(argv[i], "--output") == 0 || strcmp(argv[i], "-output") == 0
                    || strcmp(argv[i], "--out_put") == 0 || strcmp(argv[i], "-out_put") == 0){
            output = argv[i+1];
        }
        else if(strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--seed") == 0 || strcmp(argv[i], "-seed") == 0){
            seed = argv[i+1];
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

    if(not (json_path.find('/') != string::npos || json_path.find('\\') != string::npos))
        json_path = "../JSONS/" + json_path;

    arguments["json"] = json_path;
    if(not output.empty())
        arguments["output"] = output;
    if(not seed.empty())
        arguments["seed"] = seed;

    return arguments;
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