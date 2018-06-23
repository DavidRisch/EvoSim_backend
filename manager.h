#ifndef EVOSIM_MANAGER_H
#define EVOSIM_MANAGER_H

#include "header.h"

class Agent;
#include "agent.h"

class Manager {
public:

    unsigned int AgentIdCounter = 1;
    unsigned int AgentIndex = 0;
    unsigned int AgentCount = 0;

    unsigned int SelectedAgentId = 0;
    Agent* SelectedAgent = nullptr;

    double FoodToSpawn = 0;
    unsigned int FoodIndex = 0;
    unsigned int FoodCount = 0;

    double SimulationSpeed = 20;
    double LastCompleteTickTime = 0;

    string SavePath;
    string LoadPath;

    unsigned int SpawnAgent(array<double,2> spawn_position, double spawn_direction, Agent* agent);

    void AddAgent(Agent* agent);
    void RemoveAgent(Agent* agent);
    void FillToMinPopulation();

    void SpawnFood(array<double,2> spawn_position);
    void AddFood();

    void SendFrame();
    void ReceiveMessage();

    void TickAgentPartA(Agent* agent);
    void TickAgentPartB(Agent* agent);
    void Tick() ;

    void Loop();

    void Save(string path);
    void Load(string path);
};


#endif //EVOSIM_MANAGER_H
