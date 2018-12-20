#include <iostream>
#include <string>
#include <armadillo>
#include <time.h>
#include "ga.h"


using namespace std;
using namespace arma;

//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
int main(){
  srand(time(NULL));

mat DNAS=randu<mat>(100,10);
vec fitness=randu<vec>(100);
double alpha = 5.0;

DNAS = ga_eval(DNAS, fitness, alpha);

DNAS.print(" ");

return 0;
}
