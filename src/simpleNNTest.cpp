#include <iostream>
#include <string>
#include <armadillo>
#include <time.h>
#include "ga.h"
#include "simpleNN.h"


using namespace std;
using namespace arma;

int main(){

    vec inp = ones<vec>(5);
    mat inpWeights = ones<mat>(5,5);
    cube weights = ones<cube>(5,5,5);
    mat outWeights = ones<mat>(5,5);
    vec inpBiases = ones<vec>(5);
    mat biases = ones<mat>(5,5);
    vec outBiases = ones<vec>(5);


cout <<    simpleNN(inp, inpWeights , weights, outWeights , inpBiases, biases, outBiases) << endl;
cout << weights.n_elem<< endl;
//    cout << sum(a%b) << endl;

  

}
























































