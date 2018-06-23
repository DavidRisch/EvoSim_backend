#include "neuralnetwork.h"

Neuralnetwork::Neuralnetwork(vector<unsigned int>& new_nodes_in_layers){
    nodes_in_layers = new_nodes_in_layers;

    for (unsigned int i = 0; i < nodes_in_layers.size(); ++i) {
        vector<double> values (nodes_in_layers.at(i), 0.0);
        layer_values.push_back(values);

        if(i > 0){
            vector<double> previous_layer (nodes_in_layers.at(i-1), 0.0);
            vector<vector<double>> current_layer (nodes_in_layers.at(i), previous_layer);

            layer_weights.push_back(current_layer);

            vector<double> biases (nodes_in_layers.at(i), 0.0);
            layer_biases.push_back(values);
        }


    }
}

Neuralnetwork::Neuralnetwork(const Neuralnetwork& parent_neural_network){
    nodes_in_layers = parent_neural_network.nodes_in_layers;
    layer_values = parent_neural_network.layer_values;
    layer_weights = parent_neural_network.layer_weights;
    layer_biases = parent_neural_network.layer_biases;
}

array<double, NUMBER_OF_OUTPUTS> Neuralnetwork::Feed(array<double, NUMBER_OF_SENSORS> input_data) {
    for (int i = 0; i < NUMBER_OF_SENSORS; ++i) {
        layer_values[0][i] = input_data[i];
    }

    for (int current_layer = 1; current_layer < nodes_in_layers.size(); ++current_layer) {
        for (int node_in_current_layer = 0;
             node_in_current_layer < nodes_in_layers[current_layer]; ++node_in_current_layer) {

            double node = layer_biases[current_layer - 1][node_in_current_layer];

            for (int node_in_previous_layer = 0; node_in_previous_layer < nodes_in_layers[current_layer - 1]; ++node_in_previous_layer) {
                node += layer_values[current_layer - 1][node_in_previous_layer] *
                        layer_weights[current_layer - 1][node_in_current_layer][node_in_previous_layer];

            }
            node = 2 / (1 + exp(-node)) -1;  // -inf->-1  0->0  +inf->1

            layer_values[current_layer][node_in_current_layer] = node;

        }
    }

    array<double, NUMBER_OF_OUTPUTS> output {};

    for (int i = 0; i < NUMBER_OF_OUTPUTS; ++i) {
        output[i] = layer_values[nodes_in_layers.size()-1][i];
    }

    return(output);
}

void Neuralnetwork::Mutate(double sigma_biases, double sigma_weights){
    for (int current_layer = 1; current_layer < nodes_in_layers.size(); ++current_layer) {
        for (int node_in_current_layer = 0; node_in_current_layer < nodes_in_layers[current_layer];
             ++node_in_current_layer) {

            layer_biases[current_layer-1][node_in_current_layer] =
                    Methods::NormalDistribution(layer_biases[current_layer-1][node_in_current_layer], sigma_biases);

            for (int node_in_past_layer = 0; node_in_past_layer < nodes_in_layers[current_layer-1];
                 ++node_in_past_layer) {

                layer_weights[current_layer-1][node_in_current_layer][node_in_past_layer] =
                        Methods::NormalDistribution(
                                layer_weights[current_layer-1][node_in_current_layer][node_in_past_layer], sigma_weights);
            }
        }
    }
}
