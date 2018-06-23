#include "manager.h"

unsigned int Manager::SpawnAgent(array<double,2> spawn_position, double spawn_direction, Agent* agent){
    if(AgentCount == MAX_AGENTS){
        printf("Manager::AddAgent()   AgentCount == MAX_AGENTS");
        return(0);
    }

    while(AGENTS[AgentIndex]->exists){
        printf("|");
        AgentIndex += 1;
        if(AgentIndex >= MAX_AGENTS){
            AgentIndex = 0;
        }
    }

    unsigned int OriginalAgentIndex = AgentIndex;
    AGENTS[AgentIndex]->Spawn(AgentIdCounter, spawn_position, spawn_direction, agent);
    AgentIdCounter += 1;

    AgentIndex += 1;
    if(AgentIndex >= MAX_AGENTS){
        AgentIndex = 0;
    }
    AgentCount+=1;

    return(OriginalAgentIndex);
}
void Manager::AddAgent(Agent* agent){
    array<double,2> spawn_position = {
            Methods::RandomDouble(0, CONFIG.Area),
            Methods::RandomDouble(0, CONFIG.Area)
    };
    double spawn_direction = Methods::RandomDouble(0, 1);

    while(AGENTS[AgentIndex]->exists){
        printf("|");
        AgentIndex += 1;
        if(AgentIndex >= MAX_AGENTS){
            AgentIndex = 0;
        }
    }

    SpawnAgent(spawn_position, spawn_direction, agent);
}
void Manager::RemoveAgent(Agent* agent){
    agent->exists = false;
    AgentCount-=1;
}
void Manager::FillToMinPopulation(){
    while(AgentCount < CONFIG.Agent_MinPopulation){
        AddAgent(nullptr);
    }
    //printf("__%d",CONFIG.Agent_MinPopulation);
}

void Manager::SpawnFood(array<double,2> spawn_position){
    if(FoodCount >= MAX_FOODS){
        printf("Manager::AddFood()   FoodCount >= MAX_FOODS \n");
        return;
    }

    while(FOODS[FoodIndex]->exists){
        FoodIndex += 1;
        if(FoodIndex >= MAX_FOODS){
            FoodIndex = 0;
        }
        printf(".");
    }

    FOODS[FoodIndex]->position = spawn_position;
    FOODS[FoodIndex]->exists = true;

    FoodIndex += 1;
    if(FoodIndex >= MAX_FOODS){
        FoodIndex = 0;
    }
    FoodCount+=1;

}
void Manager::AddFood(){
    if(FoodCount >= MAX_FOODS-100){
        printf("Manager::AddFood()   FoodCount >= MAX_FOODS-100 \n");
        return;
    }

    array<double,2> spawn_position = {
            Methods::RandomDouble(0, CONFIG.Area),
            Methods::RandomDouble(0, CONFIG.Area)
    };
    SpawnFood(spawn_position);
}

void Manager::SendFrame(){
    json frame;
    frame["tick_count"] = CURRENT_TICK_COUNT;
    frame["save_path"] = SavePath;
    frame["load_path"] = LoadPath;

    double ticks_per_second = 1 / LastCompleteTickTime;
    frame["ticks_per_second"] = ticks_per_second;

    if(SelectedAgent){
        frame["selected_agent"] = SelectedAgent->GetInformation();
    }



    frame["agents"] = json::array();
    for(Agent* agent : AGENTS){
        if(agent->exists) {
            frame["agents"].push_back({
                                              {"id",         agent->id},
                                              {"direction",  agent->direction},
                                              {"position",   {agent->position[0], agent->position[1]}},
                                              {"age",        CURRENT_TICK_COUNT - agent->birth},
                                              {"generation", agent->generation},
                                              {"health",     agent->health},
            });
        }
    }

    frame["food_positions"] = json::array();
    for(Food* food : FOODS){
        if(food->exists) {
            frame["food_positions"].push_back({
                                                      food->position[0],
                                                      food->position[1]
            });
        }
    }

    Methods::SendMessage("updt", frame.dump());
}
void Manager::ReceiveMessage(){
    string message = Methods::ReceiveMessage();

    if(message.length() < 7){
        return;
    }

    string prefix = message.substr(0, 6);
    string content = message.substr(6);

    if(prefix == "~conf~"){
        CONFIG.Load(content);
        cout << "Configuration received" << endl;
    }else if(prefix == "~inpt~"){
        if(!CONFIG.ConfigurationSet){
            return;
        }
        json json_array = json::parse(content);

        SimulationSpeed = json_array["simulation_speed"];
        SelectedAgentId = json_array["selected_agent_id"];
        auto jump_to_tick = json_array["jump_to_tick"];
        if(jump_to_tick > CURRENT_TICK_COUNT){
            Tick();
        }

        string save_path = json_array["save_path"];
        string load_path = json_array["load_path"];

        if(!save_path.empty() and save_path != SavePath){
            Save(save_path);
            SavePath = save_path;
        }

        if(!load_path.empty() and load_path != LoadPath){
            Load(load_path);
            LoadPath = load_path;
        }

    }else{
        throw std::invalid_argument("unknown prefix");
    }
}

void Manager::TickAgentPartA(Agent* agent){
    if(!agent->exists){
        return;
    }

    array<double, NUMBER_OF_SENSORS> sensors = {
            CONFIG.Sensor_Food_Range,
            CONFIG.Sensor_Food_Range,
            CONFIG.Sensor_Food_Range,
            CONFIG.Sensor_Food_Range,
            CONFIG.Sensor_Agent_Range,
            CONFIG.Sensor_Agent_Range,
            CONFIG.Sensor_Agent_Range,
            CONFIG.Sensor_Agent_Range,
            agent->health / CONFIG.Agent_Health
    };
    agent->sensors = sensors;

    // Food
    for (int i = 0; i < MAX_FOODS; ++i) {
        if(FOODS[i]->exists) {
            array<double, 2> food_position = FOODS[i]->position;
            double distance = agent->GetDistance(food_position, CONFIG.Sensor_Food_Range);
            if (distance < 0.5f + CONFIG.Food_Diameter / 2) {
                FOODS[i]->exists=false;
                agent->Eat();
                FoodCount-=1;
            } else if (distance < CONFIG.Sensor_Food_Range) {
                agent->CalculateSensors(FOODS[i], nullptr, distance);
            }
        }
    }

    for (int i = 0; i < MAX_AGENTS; ++i) {
        if(AGENTS[i]->exists) {
            double distance = agent->GetDistance(AGENTS[i]->position, CONFIG.Sensor_Agent_Range);
            if(distance < CONFIG.Sensor_Agent_Range) {
                agent->CalculateSensors(nullptr, AGENTS[i], distance);
            }
        }
    }


    for (int i = 0; i < NUMBER_OF_SENSORS; ++i) {
        agent->sensors[i] = agent->sensors[i]/CONFIG.Sensor_Food_Range;
    }

    // Neural Network

    /*Sensors{ [Food_Left, Food_Middle, Food_Right, Food_Rest
                Agent_Left, Agent_Middle, Agent_Right, Agent_Rest
                health,]*/

    //Output{  [Rotation, Movement, Attack]

    agent->React();
    array<double, NUMBER_OF_OUTPUTS> output = agent->output;


    // Movement
    agent->direction_change = output[0] * CONFIG.Agent_Turning_MaxSpeed;

    double angle = agent->direction * 2 * PI;

    agent->position_change[0] = sin(angle) * output[1] * CONFIG.Agent_Movement_MaxSpeed;
    agent->position_change[1] = cos(angle) * output[1] * CONFIG.Agent_Movement_MaxSpeed;
    agent->health -= abs(output[1]) * CONFIG.Agent_Movement_Cost;

    // Attack
    if(output[2] > 0) {
        agent->PerformAttack();
    }

    //NaturalDecay
    agent->health -= CONFIG.Agent_NaturalDecay;

}
void Manager::TickAgentPartB(Agent* agent){
    if(!agent->exists){
        return;
    }

    agent->direction += agent->direction_change;
    agent->direction = Methods::WrapDirection(agent->direction);
    agent->position[0] += agent->position_change[0];
    agent->position[1] += agent->position_change[1];
    agent->position = Methods::WrapPosition(agent->position);

    if(agent->health <= 0){
        Manager::RemoveAgent(agent);
    }else if (agent->health >= CONFIG.Agent_Reproduce_At){
        agent->health -= CONFIG.Agent_Reproduce_Cost;
        AddAgent(agent); // AddAgent(agent);
    }

    if(agent->id == SelectedAgentId){
        SelectedAgent = agent;
    }
}
void Manager::Tick() {
    if(!CONFIG.ConfigurationSet){
        return;
    }

    CURRENT_TICK_COUNT += 1;
    //printf("!\n");
    FillToMinPopulation();

    FoodToSpawn += CONFIG.Food_PerTick;
    while (FoodToSpawn >= 1) {
        FoodToSpawn -= 1;
        AddFood();
    }

    SelectedAgent = nullptr;

    for (Agent *agent : AGENTS) {
        TickAgentPartA(agent);
    }

    for (Agent *agent : AGENTS) {
        TickAgentPartB(agent);
    }
}

void Manager::Loop(){
    while(true){
        if(SimulationSpeed == 0){
            SendFrame();
            ReceiveMessage();
            Methods::Wait(0.05);  // seconds
        }else{
            auto start = std::chrono::high_resolution_clock::now();
            //printf("+");
            Tick();
            //printf("-");

            auto one_frame_every_x_ticks = (int) floor(SimulationSpeed / FRAMES_PER_SECOND);
            //printf("one_frame_every_x_ticks %u \n",one_frame_every_x_ticks);
            if(one_frame_every_x_ticks == 0 || CURRENT_TICK_COUNT % one_frame_every_x_ticks == 0){
                //printf("*");
                SendFrame();
                ReceiveMessage();
            }



            double target_tick_time = pow(10,9) / SimulationSpeed; // in nano sec

            auto finish = std::chrono::high_resolution_clock::now();
            long long int actual_tick_time = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();  // in nano sec



            double extra_time = target_tick_time - actual_tick_time; //  in nano sec
            extra_time = extra_time/pow(10,9); //  in seconds
            //printf("##%f##",extra_time);
            LastCompleteTickTime = actual_tick_time / pow(10,9) + extra_time;

            if(extra_time > 1){
                extra_time = 1;
            }

            Methods::Wait(extra_time);

            //cout << endl << extra_time << " extra time" << endl;
        }

    }
}

void Manager::Save(string path){
    if (FILE *file = fopen(path.c_str(), "r")) {
        fclose(file);
        return;  // Save has been completed
    }
    printf("Saving...\n");

    json agents;
    for (Agent *agent : AGENTS) {
        if(agent->exists) {
            agents.push_back({
                    {"id",         agent->id},
                    {"direction",  agent->direction},
                    {"position",   {agent->position[0], agent->position[1]}},
                    {"birth",      agent->birth},
                    {"generation", agent->generation},
                    {"health",     agent->health},
                    {"neural_network", {
                        {"nodes_in_layers", agent->neural_network.nodes_in_layers},
                        {"layer_values", agent->neural_network.layer_values},
                        {"layer_weights", agent->neural_network.layer_weights},
                        {"layer_biases", agent->neural_network.layer_biases},
                    }},
            });
        }
    }

    json food_positions;
    for (Food *food : FOODS) {
        if(food->exists) {
            food_positions.push_back({food->position[0], food->position[1]});
        }
    }
    json data;
    data["tick_count"] = CURRENT_TICK_COUNT;
    data["configuration"] = CONFIG.Json;
    data["agents"] = agents;
    data["food_positions"] = food_positions;

    string json_string = data.dump();
    printf("%s\n",json_string);

    std::ofstream save_file;
    save_file.open (path);
    save_file << json_string;
    save_file.close();

}
void Manager::Load(string path){
    printf("Loading...");

    string file_content;

    std::ifstream input_file (path);
    std::stringstream buffer;
    buffer << input_file.rdbuf();
    file_content = buffer.str();

    json json_array = json::parse(file_content);

    CURRENT_TICK_COUNT = json_array["tick_count"].get<unsigned int>();

    string configuration_json = json_array["configuration"];

    CONFIG.Load(configuration_json);

    for (Agent *agent : AGENTS) {
        agent->exists = false;
    }
    AgentCount = 0;
    for(json& agent : json_array["agents"]){
        auto position = agent["position"].get<array<double, 2>>();
        auto direction = agent["direction"].get<double>();
        unsigned int agent_index = SpawnAgent(position, direction, nullptr);
        Neuralnetwork& neuralnetwork = AGENTS[agent_index]->neural_network;
        neuralnetwork.nodes_in_layers = agent["neural_network"]["nodes_in_layers"].get<vector<unsigned int>>();
        neuralnetwork.layer_values = agent["neural_network"]["layer_values"].get<vector<vector<double>>>();
        neuralnetwork.layer_weights = agent["neural_network"]["layer_weights"].get<vector<vector<vector<double>>>>();
        neuralnetwork.layer_biases = agent["neural_network"]["layer_biases"].get<vector<vector<double>>>();
    }


    for (Food *foods : FOODS) {
        foods->exists = false;
    }
    FoodCount = 0;
    for(json& food_position : json_array["food_positions"]){
        auto position = food_position.get<array<double, 2>>();
        SpawnFood(position);
    }
}
