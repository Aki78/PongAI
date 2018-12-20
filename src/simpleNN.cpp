#include <iostream>
#include <string>
#include <armadillo>
#include <time.h>
#include "ga.h"
#include "simpleNN.h"


using namespace std;
using namespace arma;

//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
vec sigmoid(vec inp){
    return 1/(1 + exp(-inp));
}
//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
vec myTanh(vec inp){
    return tanh(inp);
}
//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
rowvec sigmoidR(rowvec inp){
    return 1/(1 + exp(-inp));
}
//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
rowvec myTanhR(rowvec inp){
    return tanh(inp);
}
//------------------------------------------------------------------------------------------------------------------------
mat simpleNNmat(mat X, vec inp, mat inpWeights , cube weights, mat outWeights ,vec inpBiases, mat biases, vec outBiases){

    vec buffVec;
    buffVec = sigmoid(inpWeights*inp + inpBiases);

    int largeI = 0;

    mat Y(X.n_rows, outBiases.n_elem);


// Adding Data from values from x to each type of weights and biases.
    for(int i = 0; i < X.n_rows; i++){

        for(int dummyX = 0; dummyX < inpWeights.n_rows; dummyX++){
            for(int dummyY = 0; dummyY < inpWeights.n_cols; dummyY++){
                inpWeights.at(dummyX,dummyY) = X.at(i,largeI); 
                largeI++;
            }
        }

        for(int dummyX = 0; dummyX < weights.n_rows; dummyX++){
            for(int dummyY = 0; dummyY < weights.n_cols; dummyY++){
                for(int dummyZ = 0; dummyZ < weights.n_slices; dummyZ++){
                    weights.at(dummyX,dummyY,dummyZ) = X.at(i,largeI); 
                    largeI++;
                }
            }
        }

        for(int dummyX = 0; dummyX < outWeights.n_rows; dummyX++){
            for(int dummyY = 0; dummyY < outWeights.n_cols; dummyY++){
                outWeights.at(dummyX,dummyY) = X.at(i,largeI); 
                largeI++;
            }
        }


        for(int dummyX = 0; dummyX < inpBiases.n_rows; dummyX++){
            inpBiases.at(dummyX) = X.at(i,largeI); 
            largeI++;
        }


        for(int dummyX = 0; dummyX < biases.n_rows; dummyX++){
            for(int dummyY = 0; dummyY < biases.n_cols; dummyY++){
                biases.at(dummyX,dummyY) = X.at(i,largeI); 
                largeI++;
            }
        }

        for(int dummyX = 0; dummyX < outBiases.n_rows; dummyX++){
            outBiases.at(dummyX) = X.at(i,largeI); 
            largeI++;
        }

// Feed Forward into hidden.
        buffVec = myTanh(inpWeights*inp + inpBiases);
// Feed Forward each hidden Layer.
        for(int l = 0; l < weights.n_slices; l++){
            buffVec = myTanh(weights.slice(l)*buffVec + biases.col(l));
          }
// Feed Forward each hidden Layer.
          Y.row(i) = sigmoidR((outWeights*buffVec).t() + outBiases.t());
   largeI = 0;
   }

      return Y;
}
//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
vec simpleNNvec(vec x, vec inp, mat inpWeights , cube weights, mat outWeights ,vec inpBiases, mat biases, vec outBiases){ 

    vec buffVec;
    buffVec = sigmoid(inpWeights*inp + inpBiases);

    int largeI = 0;

    vec y(outBiases.n_elem);

// Adding Data from values from x to each type of weights and biases.
        for(int dummyX = 0; dummyX < inpWeights.n_rows; dummyX++){
            for(int dummyY = 0; dummyY < inpWeights.n_cols; dummyY++){
                inpWeights.at(dummyX,dummyY) = x.at(largeI); 
                largeI++;
            }
        }


        for(int dummyX = 0; dummyX < weights.n_rows; dummyX++){
            for(int dummyY = 0; dummyY < weights.n_cols; dummyY++){
                for(int dummyZ = 0; dummyZ < weights.n_slices; dummyZ++){

                    weights.at(dummyX,dummyY,dummyZ) = x.at(largeI); 
                    largeI++;
                }
            }
        }


        for(int dummyX = 0; dummyX < outWeights.n_rows; dummyX++){
            for(int dummyY = 0; dummyY < outWeights.n_cols; dummyY++){
                outWeights.at(dummyX,dummyY) = x.at(largeI); 
                largeI++;
            }
        }


        for(int dummyX = 0; dummyX < inpBiases.n_rows; dummyX++){
            inpBiases.at(dummyX) = x.at(largeI); 
            largeI++;
        }


        for(int dummyX = 0; dummyX < biases.n_rows; dummyX++){
            for(int dummyY = 0; dummyY < biases.n_cols; dummyY++){
                biases.at(dummyX,dummyY) = x.at(largeI); 
                largeI++;
            }
        }

        for(int dummyX = 0; dummyX < outBiases.n_rows; dummyX++){
            outBiases.at(dummyX) = x.at(largeI); 
            largeI++;
        }

// Feed Forward into hidden.
        buffVec = myTanh(inpWeights*inp + inpBiases);
// Feed Forward each hidden Layer.
        for(int l = 0; l < weights.n_slices; l++){
            buffVec = myTanh(weights.slice(l)*buffVec + biases.col(l));
          }
// Feed Forward  final Layer.
          y = sigmoid((outWeights*buffVec)  + outBiases);
      return y;
}
//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------













