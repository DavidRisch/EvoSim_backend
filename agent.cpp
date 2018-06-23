#include "agent.h"


void Agent::Spawn(unsigned int agent_id, array<double,2> spawn_position, double spawn_direction,
             Agent* parent) {
    exists = true;
    id = agent_id;
    position = spawn_position;
    direction = spawn_direction;
    health = CONFIG.Agent_Health;
    birth = CURRENT_TICK_COUNT;

    if (!parent) {
        neural_network = Neuralnetwork(CONFIG.Neural_Network_Nodes_In_Layers);
        neural_network.Mutate(CONFIG.Neural_Network_Mutate_Weights_Initial,CONFIG.Neural_Network_Mutate_Biases_Initial);
    }else{
        neural_network = Neuralnetwork(parent->neural_network);
        generation = parent->generation + 1;
        neural_network.Mutate(CONFIG.Neural_Network_Mutate_Weights_Child,CONFIG.Neural_Network_Mutate_Biases_Child);
    }
}

void Agent::Eat(){
    health += CONFIG.Food_Value;
}

void Agent::PerformAttack(){
    health -= CONFIG.Agent_Attack_Cost;

    for( Agent* agent : AGENTS){
        if(agent->exists && agent->id != id){
            double distance = GetDistance(agent->position, CONFIG.Agent_Attack_Range);
            if(distance < CONFIG.Agent_Attack_Range){
                double relative_direction = Methods::CalculateDirectionDifference(&position, &agent->position, direction);
                double size_middle = CONFIG.Agent_Attack_Angle / 360;
                if (relative_direction < size_middle / 2 || relative_direction > (1 - size_middle / 2)){
                    health += agent->GetAttacked();
                }
            }
        }
    }
};

double Agent::GetAttacked(){
    double health_to_give = CONFIG.Agent_Attack_Damage;
    if(health_to_give > health){
        health_to_give = health;
    }
    if(health_to_give < 0){
        health_to_give = 0;
    }

    health -= CONFIG.Agent_Attack_Damage;

    return(health_to_give);
}

double Agent::GetDistance(array<double,2> other_position, double max_range){
    double distance_x = position[0] - other_position[0];
    double distance_y = position[1] - other_position[1];

    double distance =  sqrt(pow(distance_x, 2.0) + pow(distance_y, 2.0));

    return(distance);

    /*if(abs(distance_x) <= max_range and abs(distance_y) <= max_range){
            distance = math.sqrt(math.pow(distance_x, 2) + math.pow(distance_y, 2))
    return distance
    else:
    return 999999*/
}
json Agent::GetInformation(){
    json json_array;

    json_array["position"] = 5.01;
    json_array["health"] = health;
    json_array["age"] = CURRENT_TICK_COUNT - birth;
    json_array["generation"] = generation;
    json_array["sensors"] = sensors;
    json_array["output"] = output;

    return json_array;
}

void Agent::React(){
    output = neural_network.Feed(sensors);
}

void Agent::CalculateSensors(Food* food, Agent* agent, double distance) {
    double size_middle;
    double size_side;
    array<double, 2> *other_position;
    unsigned int sensor_index_offset;

    if (food) {
        size_middle = CONFIG.Sensor_Food_Middle_Angel / 360;
        size_side = CONFIG.Sensor_Food_Side_Angel / 360;
        other_position = &food->position;
        sensor_index_offset = 0;
    } else {
        if(agent->id == id){
            return;
        }
        size_middle = CONFIG.Sensor_Agent_Middle_Angel / 360;
        size_side = CONFIG.Sensor_Agent_Side_Angel / 360;
        other_position = &agent->position;
        sensor_index_offset = 4;
    }

    double relative_direction = Methods::CalculateDirectionDifference(&position, other_position, direction);

    unsigned int sensor_index;

    if (1 - size_middle / 2 - size_side < relative_direction && relative_direction < 1 - size_middle / 2){
        sensor_index = 0; // left
    }else if (relative_direction < size_middle / 2 || relative_direction > (1 - size_middle / 2)){
        sensor_index = 1; // middle
    }else if (size_middle / 2 < relative_direction && relative_direction < size_middle / 2 + size_side){
        sensor_index = 2; // right
    }else{
        sensor_index = 3; // rest
    }

    if(sensors[sensor_index + sensor_index_offset] > distance){
        sensors[sensor_index + sensor_index_offset] = distance;
    }
}
