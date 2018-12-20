#include <iostream>
#include <string>
#include <armadillo>
#include <time.h>
#include "ga.h"
#include "simpleNN.h"

using namespace std;
using namespace arma;

///#define SIZEOF(_arr) (sizeof(_arr)/sizeof(_arr[0]))

using namespace arma;

int main(int argc,char** argv){  srand(time(NULL));

  rowvec myx= linspace<rowvec>(0.0,2*3.1516,100);
  rowvec mysin= sin(myx);

  int population = 100;

  int iMax = atoi(argv[1]);
  double temperature = 10.0;
  vec fitness=zeros<vec>(population);

  vec inp = ones<vec>(1);

  vec bestScore = 1000000*ones<vec>(1);

  int inpSize = inp.n_elem;
  int height = 2;
  int depth = 2;
  int outputSize = 1;
  int iMaxTemp = iMax;

  string runName = argv[2];

  mat inpWeights = ones<mat>(height,inpSize);
  cube weights = ones<cube>(height,height,depth);
  mat outWeights = ones<mat>(outputSize,height);

  vec inpBiases = ones<vec>(height);
  mat biases = ones<mat>(height,depth);
  vec outBiases = ones<vec>(outputSize);
  double totalMax = 10000;

  vec buffVec = sigmoid(inpWeights*inp + inpBiases);
  double reg = 0.000005;

  int outerMax = 10;

  mat DNAS=1 - 2*randu<mat>(fitness.n_elem, inpWeights.n_elem + weights.n_elem + outWeights.n_elem + inpBiases.n_elem + biases.n_elem + outBiases.n_elem);

  mat outputs = zeros<mat>(fitness.n_elem,outBiases.n_elem);
  vec outputsFinal = zeros<vec>(outBiases.n_elem);
  vec finalResult(myx.n_elem);

  double alpha = 10.0;
  double punish = 0;
  double bestFitt = 100000000;
  vec    finalWeights;
  int decTemp = 0;

  for(int outer=0; outer < outerMax; outer++){ 
   decTemp = 0;

    DNAS=1 - 2*randu<mat>(fitness.n_elem, inpWeights.n_elem + weights.n_elem + outWeights.n_elem + inpBiases.n_elem + biases.n_elem + outBiases.n_elem);
    temperature = 10.0;

    for(int i=0; i < iMaxTemp; i++){ 

//      temperature -= 10.0/ iMax;
      temperature = 10.0 * exp( - (double) decTemp / (double) iMax / 2.0);
      decTemp++;
      
      fitness=zeros<vec>(population);

      for(int xInt = 0; xInt < myx.n_elem; xInt++){
    
        inp(0) = myx(xInt);
        outputs = simpleNNmat(DNAS,inp, inpWeights, weights, outWeights, inpBiases, biases, outBiases);

        for(int f = 0; f < fitness.n_elem; f++){

          punish = sum(abs(outputs.row(f) - mysin(xInt))) + reg*sum(abs(DNAS.row(f))) ;
          fitness(f) = fitness(f) + punish ; // changeable

        }
      }

      DNAS = ga_eval(DNAS, fitness, alpha,0.000000000, temperature);
//      DNAS.save("DNASrerun.dat",raw_ascii);

      if(bestFitt > min(fitness)){
       
        cout << "hit: " << bestFitt/myx.n_elem <<endl;
        bestScore(0) = bestFitt/myx.n_elem;
        finalWeights = DNAS.row(index_min(fitness)).t();
        bestFitt = min(fitness);
        finalWeights.save("LastFinalWeights" + runName + ".save",raw_ascii);
        finalWeights.save("LastFinalWeightsBak.save",raw_ascii);
        bestScore.save("LastBestScore" + runName + ".save",raw_ascii);

        for(int i = 0; i < myx.n_elem; i++){

          inp(0) = myx(i);
          outputsFinal= simpleNNvec(finalWeights,inp, inpWeights, weights, outWeights, inpBiases, biases, outBiases);
          finalResult(i) = outputsFinal(0);

        }

        finalResult.save("sinDataFinal" + runName + ".dat",raw_ascii);
//        iMaxTemp = i + iMax;
          i = i - 10*outer - 1;

      }

      cout << i << " min fitness is: " << min(fitness)/myx.n_elem <<  " Temperature is: "<< temperature  << " C" << endl;

    }

  }


return 0;

}






