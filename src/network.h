#ifndef myNetwork
#define myNetwork

#pragma once
#include <signal.h>

#define BREAKPOINT (raise(SIGINT))
#include <stddef.h>

typedef void (*kernel)(int size, double* data);

struct ActivationFunction {
    enum Enum {
        RELU,
        LINEAR,
        SIGMOID,
        TANH
    };
    bool max_bounded;
    double max_val;
    bool min_bounded;
    double min_val;
    double null_value;
    kernel activation;
    kernel activation_prime;
};

ActivationFunction getActivationFunction(ActivationFunction::Enum func);

///Data structure for a neural layer
typedef struct Layer {

  /// Number of neurons in the layer
  int nN;
  /// Number of inputs connected to the layer
  int nInputs;
  int nOutputs;
  
  /// Neuron outputs
  double *outputs;

  double *weights;
  double *biases;
  
  ActivationFunction::Enum activation_enum;
  kernel activation;
  kernel activation_prime;
} Layer;

void layer_compute_weights(Layer* layer);
void layer_compute(Layer* layer);

/*
Networks contains nLayers = 1 + nHiddenLayers.
Parameters layout is [layer 0 weights|layer 0 bias| layer 1 weights...].
*/

typedef struct Network {

    ///number of inputs, outputs and hidden layers
    int nInputs;
    int nOutputs;
    int nHiddenLayers;
 
    /// Pointer to the inputs array
    const double *inputs;
    double *outputs;

    /// Network weights, biases
    double* parameters;
    int parameter_size;
 
    /// Number of layers
    int nLayers;
    /// List of layers
    Layer *layers;
} Network;

/*Creates a network. Should be released with network_free. 
Network must contain atleast one layer (output). Other layers are hidden.*/
Network* network_create(int input_size, 
    int num_layers, const int* layer_sizes, 
    int num_activations, const ActivationFunction::Enum* func);

void network_load_parameters(Network* n, 
    int parameter_size, 
    const double* parameters);

/*Feedforward through all layers. After this network_output gives the result*/
void network_compute(Network* network, int size, const double* input);
void network_destroy(Network* network);

/*If output size is defined it has the output size*/
const double* network_output(Network* n, int* output_size = NULL);

/*Armadillo interface for convinience*/
#include <armadillo>
void network_load_parameters(Network* n, const arma::vec& parameters);
void network_compute(Network* n, const arma::vec& input);
void network_output(Network* n, arma::vec& output);

/*Simple backpropagation for testing purposes*/
void network_backpropagate(Network* n, const double* input, const double* result, double rate);
/*Simple GA*/
void network_simple_ga();

#endif
