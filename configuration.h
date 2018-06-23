#ifndef EVOSIM_CONFIGURATION_H
#define EVOSIM_CONFIGURATION_H

#include "header.h"

#include <json.hpp>   // https://github.com/nlohmann/json

using json = nlohmann::json;

class Configuration {
public:
    bool ConfigurationSet = false;
    string Json;

    unsigned int Area;
    double Agent_Health;
    double Agent_Movement_MaxSpeed;
    double Agent_Movement_Cost;
    double Agent_Turning_MaxSpeed;
    double Agent_Attack_Range;
    double Agent_Attack_Angle;
    double Agent_Attack_Damage;
    double Agent_Attack_Cost;
    double Agent_Attack_Gain;
    double Agent_NaturalDecay;
    unsigned int Agent_MinPopulation;
    double Agent_Reproduce_At;
    double Agent_Reproduce_Cost;
    double Food_Value;
    double Food_Diameter;
    double Food_PerTick;
    double Sensor_Food_Range;
    double Sensor_Food_Middle_Angel;
    double Sensor_Food_Side_Angel;
    double Sensor_Agent_Range;
    double Sensor_Agent_Middle_Angel;
    double Sensor_Agent_Side_Angel;
    std::vector<unsigned int> Neural_Network_Nodes_In_Layers;
    double Neural_Network_Mutate_Weights_Initial;
    double Neural_Network_Mutate_Biases_Initial;
    double Neural_Network_Mutate_Weights_Child;
    double Neural_Network_Mutate_Biases_Child;

    void Load(std::string& json_string);
};

#endif //EVOSIM_CONFIGURATION_H
