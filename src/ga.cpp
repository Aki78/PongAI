#include <iostream>
#include <string>
#include <armadillo>
#include <time.h>
#include "ga.h"


using namespace std;
using namespace arma;

//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
mat ga_eval(mat DNAS, vec fitness, double alpha, double reg, double temperature){ 

  int DNASrows = DNAS.n_rows;
  int DNAScols = DNAS.n_cols;
//  fitness *= 1/max(abs(fitness));

  double myMax=max(max(abs(DNAS)));

	double randBuff=0;
  double aveDNAS=sum(sum(abs(DNAS),0)) /DNASrows/DNAScols;

	double mutRat=0.1*temperature;//*DNAScols;
	double mutAmmount=0.001*aveDNAS*temperature;

  double bigMutRat=0.001*temperature;//*DNAScols;
	double bigMutAmmount=0.005*aveDNAS*temperature;

  double veryBigMutRat=0.0001*temperature;//*DNAScols;
	double verBigMutAmmount=1.0*aveDNAS*temperature;

	int decision[2*DNASrows];
	int myindex;

  rowvec baby = zeros<rowvec>(DNAScols);
	vec orderedV(DNASrows);

	mat orderedDNAS(DNASrows,DNAScols);
	mat newDNAS(DNASrows,DNAScols);
  // without this activation, higher fitness the better.
 fitness.save("fitness.save",raw_ascii);
	fitness=-fitness - reg*sum(DNAS%DNAS,1);
 vec regVec = reg*sum(DNAS%DNAS,1);
 regVec.save("regVec.save",raw_ascii);


	uvec q = sort_index(fitness,"descend");

  for(int j=0;j < DNASrows; j++){
     orderedDNAS.row(j)= DNAS.row(q(j)); 
	   orderedV(j) = fitness(q(j));
	} 

 
 fitness.save("fitness2.save",raw_ascii);
 q.save("q.save",raw_ascii);
 vec y = exp(alpha*orderedV)/sum(exp(alpha*orderedV));
 y.save("y.save",raw_ascii);
 vec z = cumsum(y); 
 z.save("z.save",raw_ascii);

 y.save("distri.dat",raw_ascii);
 z.save("cumsum.dat",raw_ascii);

  for(int j=0;j < 2*DNASrows; j++){
    myindex = index_min(abs(z - (double)(rand() + 1)/(RAND_MAX))  );
    decision[j] = myindex;
  }

    //BEGIN MATING RITUAL	
	  for(int i=0; i < 2*DNAS.n_rows-1; i = i + 2){
		//cout << "THE I" << (i)/2 << endl;
       baby =  orderedDNAS.row(decision[i]);
	   //MIXING
	  for(int j=0; j <DNAScols; j++){
       if((double)(rand() + 1)/(RAND_MAX) < 0.5) baby(j) = orderedDNAS.row(decision[i+1])(j);
	  }
	  //SMALL MUTATION
	  for(int j=0; j <DNAScols; j++){
       randBuff = (double)(rand() + 1)/(RAND_MAX);
       if((double)(rand() + 1)/(RAND_MAX) < mutRat) baby(j) = baby(j) + baby(j) * mutAmmount*(2*randBuff-1);
	  }
	  //LARGE MUTATION
	  for(int j=0; j <DNAScols; j++){
       randBuff = (double)(rand() + 1)/(RAND_MAX);
       if((double)(rand() + 1)/(RAND_MAX) < bigMutRat) baby(j) = baby(j) + baby(j) * bigMutAmmount*(2*randBuff-1);
	  }
	  for(int j=0; j <DNAScols; j++){
       randBuff = (double)(rand() + 1)/(RAND_MAX);
       if((double)(rand() + 1)/(RAND_MAX) < veryBigMutRat) baby(j) = baby(j) + baby(j) * verBigMutAmmount*(2*randBuff-1);
	  }

	newDNAS.row(i/2) = baby; 
	}


	return newDNAS;
}
//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------

























































