#ifndef EVOSIM_METHODS_H
#define EVOSIM_METHODS_H

#include "header.h"

#include <thread>
#include <fstream>
#include <sstream>
#include <json.hpp>   // https://github.com/nlohmann/json

using json = nlohmann::json;

#ifdef WINDOWS
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
#endif

class Methods {
public:
    static std::array<double, 2> WrapPosition(std::array<double, 2> position);
    static double WrapDirection(double direction);
    static double CalculateDirectionDifference(array<double, 2>* position_a, array<double, 2>* position_b, double direction_a);
    static double NormalDistribution(double center, double sigma);
    static void Wait(double seconds);
    static double RandomDouble(double min, double max);
    static void SendMessage(const std::string& header, const std::string& content);
    static string ReceiveMessage();
};


#endif //EVOSIM_METHODS_H
