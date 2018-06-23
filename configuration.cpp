#include "configuration.h"

void Configuration::Load(string& json_string){
    ConfigurationSet = true;
    Json = json_string;

    json json_array = json::parse(json_string);

    Area = json_array["Area"];
    Agent_Health = json_array["Agent_Health"];
    Agent_Movement_MaxSpeed = json_array["Agent_Movement_MaxSpeed"];
    Agent_Movement_Cost = json_array["Agent_Movement_Cost"];
    Agent_Turning_MaxSpeed = json_array["Agent_Turning_MaxSpeed"];
    Agent_Attack_Range = json_array["Agent_Attack_Range"];
    Agent_Attack_Angle = json_array["Agent_Attack_Angle"];
    Agent_Attack_Damage = json_array["Agent_Attack_Damage"];
    Agent_Attack_Cost = json_array["Agent_Attack_Cost"];
    Agent_Attack_Gain = json_array["Agent_Attack_Gain"];
    Agent_NaturalDecay = json_array["Agent_NaturalDecay"];
    Agent_MinPopulation = json_array["Agent_MinPopulation"];
    Agent_Reproduce_At = json_array["Agent_Reproduce_At"];
    Agent_Reproduce_Cost = json_array["Agent_Reproduce_Cost"];
    Food_Value = json_array["Food_Value"];
    Food_Diameter = json_array["Food_Diameter"];
    Food_PerTick = json_array["Food_PerTick"];
    Sensor_Food_Range = json_array["Sensor_Food_Range"];
    Sensor_Food_Middle_Angel = json_array["Sensor_Food_Middle_Angel"];
    Sensor_Food_Side_Angel = json_array["Sensor_Food_Side_Angel"];
    Sensor_Agent_Range = json_array["Sensor_Agent_Range"];
    Sensor_Agent_Middle_Angel = json_array["Sensor_Agent_Middle_Angel"];
    Sensor_Agent_Side_Angel = json_array["Sensor_Agent_Side_Angel"];
    Neural_Network_Nodes_In_Layers = json_array["Neural_Network_Nodes_In_Layers"].get<std::vector<unsigned int>>();
    Neural_Network_Mutate_Weights_Initial = json_array["Neural_Network_Mutate_Weights_Initial"];
    Neural_Network_Mutate_Biases_Initial = json_array["Neural_Network_Mutate_Biases_Initial"];
    Neural_Network_Mutate_Weights_Child = json_array["Neural_Network_Mutate_Weights_Child"];
    Neural_Network_Mutate_Biases_Child = json_array["Neural_Network_Mutate_Biases_Child"];
}
