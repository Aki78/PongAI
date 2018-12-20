#include <iostream>
#include <string>
#include <armadillo>
#include <time.h>


using namespace std;
using namespace arma;

//------------------------------------------------------------------------------------------------------------------------
vec sigmoid(vec inp);
//------------------------------------------------------------------------------------------------------------------------
vec simpleNN(vec inp, mat inpWeights , cube weights, mat outWeights ,vec inpBiases, mat biases, vec outBiases);
//------------------------------------------------------------------------------------------------------------------------
mat simpleNNmat(mat X, vec inp, mat inpWeights , cube weights, mat outWeights ,vec inpBiases, mat biases, vec
outBiases);
//------------------------------------------------------------------------------------------------------------------------
vec simpleNNvec(vec x, vec inp, mat inpWeights , cube weights, mat outWeights ,vec inpBiases, mat biases, vec
outBiases);
//------------------------------------------------------------------------------------------------------------------------

























































