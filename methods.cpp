#include "methods.h"

array<double, 2> Methods::WrapPosition(array<double, 2> position){
    for (int i = 0; i <= 1; ++i) {
        if(position[i] < 0) {
            position[i] = CONFIG.Area + position[i];
        }
        if(position[i] > CONFIG.Area){
            position[i] = CONFIG.Area - position[i];
        }
    }

    return(position);
}

double Methods::WrapDirection(double direction) {
    while (direction < 0) {
        direction = direction + 1;
    }
    while (direction > 1) {
        direction = direction - 1;
    }

    return(direction);
}

double Methods::CalculateDirectionDifference(array<double, 2>* position_a, array<double, 2>* position_b, double direction_a){
    double a_to_b_x = position_b->at(0) - position_a->at(0);
    double a_to_b_y = position_b->at(1) - position_a->at(1);

    double angle = (atan2(-a_to_b_y, -a_to_b_x) + PI) / 2;

    double direction = angle / PI;
    direction = -direction + 0.25;

    direction = direction - direction_a;
    while(direction < 0) {
        direction = 1 + direction;
    }

    return(direction);
}

double Methods::NormalDistribution(double center, double sigma){
    std::normal_distribution<double> distribution(center, sigma);

    double number = distribution(RANDOM_GENERATOR);
    return(number);
}

void Methods::Wait(double seconds){
    auto milliseconds = (int) (seconds * 1000);
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

double Methods::RandomDouble(double min, double max){
    return (double) rand() / RAND_MAX * (max - min) + min; // NOLINT
}

void Methods::SendMessage(const string& header, const string& content){
    string message = "~" + header + "~ " + content;

    //printf(":");

    std::ofstream output_file;
    output_file.open (COMMUNICATION_PATH + "ToFrontend.txt");
    output_file << message;
    output_file.close();
}

string Methods::ReceiveMessage(){
    string message;

    std::ifstream input_file (COMMUNICATION_PATH + "ToBackend.txt");
    std::stringstream buffer;
    buffer << input_file.rdbuf();
    message = buffer.str();


    std::ofstream output_file;
    output_file.open (COMMUNICATION_PATH + "ToBackend.txt");
    output_file << "";
    output_file.close();

    return(message);

}
