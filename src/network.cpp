#include "network.h"
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <assert.h>
#include <armadillo>
#include <iostream>

void relu_kernel(int size, double* data);
void relu_prime_kernel(int size, double* data);
void lin_kernel(int size, double* data);
void lin_prime_kernel(int size, double* data);
void sigmoid_kernel(int size, double* data);
void sigmoid_prime_kernel(int size, double* data);
void tanh_kernel(int size, double* data);
void tanh_prime_kernel(int size, double* data);

static const ActivationFunction activation_functions[] = {
    {false, 0.0, true, 0.0, -1.0, &relu_kernel, &relu_prime_kernel},
    {false, 0.0, false, 0.0, 0.0, &lin_kernel, &lin_prime_kernel},
    {true, 1.0, true, 0.0, -1e46, &sigmoid_kernel, &sigmoid_prime_kernel},
    {true, 1.0, true, -1.0, -1e46, &tanh_kernel, &tanh_prime_kernel}
};

ActivationFunction getActivationFunction(ActivationFunction::Enum func) {
    return activation_functions[func];
}

void layer_init(Layer* layer, int sI, int sO, ActivationFunction::Enum func, double* parameters) {
    layer->nInputs = sI;
    layer->nOutputs = sO;
    layer->nN = sO;
    layer->activation = activation_functions[func].activation;
    layer->activation_prime = activation_functions[func].activation_prime;
    layer->activation_enum = func;
    layer->outputs = (double*)calloc(sO, sizeof *layer->outputs);
    layer->weights = parameters;
    layer->biases = parameters + sI*sO;
}

void layer_free(Layer* layer) {
    free(layer->outputs);
}

void layer_compute_z(Layer* layer, const double* inputs) {
    arma::mat weights(layer->weights, layer->nOutputs, layer->nInputs, false, true);
    arma::colvec bias(layer->biases, layer->nOutputs, false, true);
    arma::colvec output(layer->outputs, layer->nOutputs, false, true);
    arma::colvec input((double*)inputs, layer->nInputs, false, true);
    output = weights*input + bias;
}

void layer_compute_activation(Layer* layer, const double* inputs) {
    layer_compute_z(layer, inputs);
    layer->activation(layer->nOutputs, layer->outputs);
}


Network* network_create(int input_size, int num_layers, const int* layer_sizes, 
    int num_activations, const ActivationFunction::Enum* func) {
    assert(input_size > 0);
    assert(num_layers > 0);
    assert(layer_sizes != NULL);
    assert(func != NULL);
    Network* n = (Network*)calloc(1, sizeof *n);
    int layers = num_layers;
    n->nLayers = layers;
    n->nHiddenLayers = layers - 1;
    n->layers = (Layer*)calloc(layers, sizeof *n->layers);
    n->parameter_size = input_size*layer_sizes[0];
    for(int i = 1; i < layers; ++i) {
        n->parameter_size += layer_sizes[i-1]*layer_sizes[i] + layer_sizes[i];
    }
    n->parameters = (double*)calloc(n->parameter_size, sizeof *n->parameters);
    layer_init(n->layers, input_size, layer_sizes[0], func[0], n->parameters);
    int parameter_index = input_size*layer_sizes[0];
    for(int i = 1; i < layers; ++i) {
        layer_init(&n->layers[i], layer_sizes[i-1], layer_sizes[i], func[i],
            &n->parameters[parameter_index]);
        parameter_index += layer_sizes[i-1]*layer_sizes[i] + layer_sizes[i];
    }
    n->nInputs = input_size;
    n->nOutputs = layer_sizes[num_layers-1];
    n->outputs = n->layers[layers-1].outputs;
    return n;
}


void network_load_parameters(Network* n, int parameter_size, const double* parameters) {
    assert(parameter_size == n->parameter_size);
    //Copy the parameters to network (copying not always needed?)
    //memcpy(n->parameters, parameters, parameter_size*sizeof(*parameters));
    for(int i = 0; i < parameter_size; ++i)
        n->parameters[i] = parameters[i];
}
void network_compute(Network* n, int size, const double* input) {
    assert(size == n->nInputs);
    layer_compute_activation(&n->layers[0], input);
    for(int i = 1; i < n->nLayers; ++i) {
        layer_compute_activation(&n->layers[i], n->layers[i-1].outputs);
    }
}

void network_destroy(Network* n) {
    for(int i = 0; i < n->nLayers; ++i) {
        layer_free(&n->layers[i]);
    }
    free(n->layers);
    free(n->parameters);
    free(n);
}

const double* network_output(Network* n, int* output_size) {
    if(output_size != NULL)
        *output_size = n->nOutputs;
    return n->outputs;
}

/*Armadillo interface*/
void network_load_parameters(Network* n, const arma::vec& parameters) {
    network_load_parameters(n, parameters.n_rows, parameters.memptr());
}
void network_compute(Network* n, const arma::vec& input) {
    network_compute(n, input.n_rows, input.memptr());
}
void network_output(Network* n, arma::vec& output) {
    output = arma::colvec(n->outputs, n->nOutputs, true);
}

/*Backpropagation specifics*/

void layer_update_parameters(Layer* l, const double* input, double* delta, double rate) {
    //Outer product
    for(int j = 0; j < l->nInputs; ++j) {
        double input_ = input[j];
        for(int k = 0; k < l->nOutputs; ++k) {
            l->weights[j*l->nOutputs + k] += rate*input_*delta[k];
        }
    }
    for(int k = 0; k < l->nOutputs; ++k)
        l->biases[k] += rate*delta[k];
}

/*Function overwrites layer_j_activation_prime with new delta*/
void layer_backpropagate_delta( Layer* kl, Layer* jl, double* layer_j_activation_prime, double* delta) 
{
    double* new_delta = layer_j_activation_prime;
    for(int j = 0; j < jl->nOutputs; ++j) {
        for(int k = 0; k < kl->nOutputs; ++k) {
            new_delta[j] *= delta[k]*kl->weights[j*kl->nOutputs + k];
        }
    }
}

void network_backpropagate(Network* n, const double* inputs, const double* result, double rate) {

    double** temps = (double**)calloc(n->nLayers, sizeof (double*));
    for(int i = 0; i < n->nLayers; ++i)
        temps[i] = (double*)calloc(n->layers[i].nOutputs, sizeof(double));
    //Feedforward and take the z_s (values before activation) into memory
    layer_compute_z(&n->layers[0], inputs);
    memcpy(temps[0], n->layers[0].outputs, n->layers[0].nOutputs*sizeof(double));
    n->layers[0].activation(n->layers[0].nOutputs, n->layers[0].outputs);
    for(int i = 1; i < n->nLayers; ++i) {
        Layer* l = &n->layers[i];
        layer_compute_z(l, n->layers[i-1].outputs);
        memcpy(temps[i], l->outputs, l->nOutputs*sizeof(double));
        l->activation(l->nOutputs, l->outputs);
    }
    
    //Backpropagate, at each step one z is replaced with backpropagated delta
    Layer* last = &n->layers[n->nLayers-1];
    last->activation_prime(last->nOutputs, temps[n->nLayers-1]);
    for(int i = 0; i < last->nOutputs; ++i)
        temps[n->nLayers-1][i] *= (result[i] - n->outputs[i]);
    for(int i = n->nLayers-1; i > 0; --i) {
        Layer* k = &n->layers[i];
        Layer* j = &n->layers[i-1];
        layer_update_parameters(k, j->outputs, temps[i], rate);
        j->activation_prime(j->nOutputs, temps[i-1]);
        layer_backpropagate_delta(k, j, temps[i-1], temps[i]);
    }
    layer_update_parameters(&n->layers[0], inputs, temps[0], rate);
    
    for(int i = 0; i < n->nLayers; ++i)
        free(temps[i]);
    free(temps);
}


void relu_kernel(int size, double* data) {
    for(int i = 0; i < size; ++i) {
        data[i] = std::max(0.0, data[i]);
    }
}

void relu_prime_kernel(int size, double* data) {
    for(int i = 0; i < size; ++i) {
        data[i] = data[i] < 0.0 ? 0.0 : 1.0;
    }
}

void lin_kernel(int size, double* data) {
    return;
}

void lin_prime_kernel(int size, double* data) {
    for(int i = 0; i < size; ++i)
        data[i] = 1.0;
}

void sigmoid_kernel(int size, double* data) {
    for(int i = 0; i< size; ++i) {
        data[i] = 1.0/(1.0 + exp(-data[i]));
    }
}

void sigmoid_prime_kernel(int size, double* data) {
    for(int i = 0; i < size; ++i) {
        double f = 1.0/(1.0 + exp(-data[i]));
        data[i] = f*(1-f);
    }
}

void tanh_kernel(int size, double* data) {
    for(int i = 0; i < size; ++i) {
        data[i] = tanh(data[i]);
    }
}

void tanh_prime_kernel(int size, double* data) {
    for(int i = 0; i < size; ++i) {
        data[i] = 1.0 - pow(tanh(data[i]), 2);
    }
}