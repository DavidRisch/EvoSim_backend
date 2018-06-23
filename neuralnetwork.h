#ifndef EVOSIM_NEURALNETWORK_H
#define EVOSIM_NEURALNETWORK_H

#include "header.h"


class Neuralnetwork {
public:

    vector<unsigned int> nodes_in_layers;

    vector<vector<double>> layer_values;          // [layer][node]
    vector<vector<vector<double>>> layer_weights; // [layer-1][node_target][node_origin]
    vector<vector<double>> layer_biases;          // [layer-1][node]

    explicit Neuralnetwork(vector<unsigned int>& nodes_in_layers);
    Neuralnetwork() = default;
    Neuralnetwork(const Neuralnetwork& parent_neural_network);
    array<double, NUMBER_OF_OUTPUTS> Feed(array<double, NUMBER_OF_SENSORS> input_data);
    void Mutate(double sigma_biases, double sigma_weights);
};

#endif //EVOSIM_NEURALNETWORK_H
