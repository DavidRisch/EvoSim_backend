#ifndef EVOSIM_HEADER_H
#define EVOSIM_HEADER_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <cmath>
#include <random>
#include "libraries/json.hpp"   // https://github.com/nlohmann/json

using std::vector;
using std::array;
using std::string;
using std::cout;
using std::endl;
using json = nlohmann::json;

#include "configuration.h"
#include "methods.h"


#define PI          3.141592653589793238462643383279502884

#define NUMBER_OF_SENSORS 9
#define NUMBER_OF_OUTPUTS 3
#define MAX_AGENTS 512
#define MAX_FOODS 1024
#define FRAMES_PER_SECOND 10

class Configuration;
class Manager;
class Agent;
class Food{
public:
    bool exists = false;
    array<double, 2> position {0, 0};
};

extern Configuration& CONFIG;
extern Manager* MANAGER;
extern array<Agent*, MAX_AGENTS> AGENTS;
extern array<Food*, MAX_FOODS> FOODS;
extern std::string COMMUNICATION_PATH;
extern unsigned int CURRENT_TICK_COUNT;
extern std::default_random_engine& RANDOM_GENERATOR;

#endif //EVOSIM_HEADER_H
