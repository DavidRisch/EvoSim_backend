#ifndef EVOSIM_AGENT_H
#define EVOSIM_AGENT_H

#include "header.h"
#include "neuralnetwork.h"


class Agent {
public:
    bool exists = false;

    unsigned int id = 0;
    array<double,2> position {0, 0};
    double direction = 0;  // 0 to 1 clockwise, 0 is facing up
    array<double,2> position_change {0, 0};
    double direction_change = 0;
    double health = 0;
    unsigned int birth = 0;
    Neuralnetwork neural_network;
    array<double, NUMBER_OF_SENSORS> sensors = {};
    array<double, NUMBER_OF_OUTPUTS> output = {};
    unsigned int generation = 0;

    Agent() = default;
    void Spawn(unsigned int agent_id, array<double,2> spawn_position, double spawn_direction,
               Agent* parent);
    void Eat();
    void PerformAttack();
    double GetAttacked();
    double GetDistance(array<double,2> other_position, double max_range);
    json GetInformation();
    void React();
    void CalculateSensors(Food* food, Agent* agent, double distance);
};

#endif //EVOSIM_AGENT_H
