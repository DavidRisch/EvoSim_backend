#include "header.h"

#include "manager.h"

Configuration Config = Configuration();
Configuration& CONFIG = Config;

array<Agent*, MAX_AGENTS> AGENTS;
array<Food*, MAX_FOODS> FOODS;

array<Agent, MAX_AGENTS> Agents;
array<Food, MAX_FOODS> Foods;

string COMMUNICATION_PATH;
unsigned int CURRENT_TICK_COUNT = 0;


Manager MyManger;
Manager* MANAGER = &MyManger;

std::default_random_engine RandomGenerator(0);
std::default_random_engine& RANDOM_GENERATOR = RandomGenerator;



int main(){
    cout << "Compiled on: " << __DATE__ << "  " << __TIME__ << endl;

    auto Seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine RandomGenerator(Seed);
    RANDOM_GENERATOR = RandomGenerator;

    char cCurrentPath[FILENAME_MAX];
    GetCurrentDir(cCurrentPath, sizeof(cCurrentPath));
    string communication_path = cCurrentPath;
    unsigned int pos = communication_path.find("EvoSim_backend");
    communication_path = communication_path.substr(0, pos);
    cout << (communication_path + "") <<endl;


    COMMUNICATION_PATH = communication_path;
    CONFIG = Configuration();



    while(!CONFIG.ConfigurationSet){
        Methods::SendMessage("conf", "{}");
        MANAGER->ReceiveMessage();
        Methods::Wait(0.05);
    }

    for (int i = 0; i < MAX_AGENTS; ++i) {
        AGENTS[i] = &Agents[i];
    }

    for (int i = 0; i < MAX_FOODS; ++i) {
        FOODS[i] = &Foods[i];
    }


    cout << "starting loop()" << endl;
    MANAGER->Loop();

}
