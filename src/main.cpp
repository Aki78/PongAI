#include "game.h"
#include <QApplication>
#include <QtWidgets>
#include <QThread>
#include<armadillo>
#include"simpleNN.h"
#include"ga.h"

int main(int argc, char *argv[])
{
    srand (time(NULL));
   int population = 100;
   int generations = 10;
   double temperature = 0.000000;
   arma::vec fitness =  arma::zeros<arma::vec>(population);
   arma::vec bestDNA;

   arma::vec inpNN = arma::ones<arma::vec>(8);
   int inputScore =0;
   int enemyScore =0;

   int inpSize = inpNN.n_elem;
   int height = 10;
   int depth = 10;
   int outputSize = 2;

   arma::mat inpWeights = arma::randu<arma::mat>(height,inpSize);
   arma::cube weights = arma::randu<arma::cube>(height,height,depth);
   arma::mat outWeights = arma::randu<arma::mat>(outputSize,height);
   arma::vec inpBiases = arma::randu<arma::vec>(height);
   arma::mat biases = arma::randu<arma::mat>(height,depth);
   arma::vec outBiases = arma::randu<arma::vec>(outputSize);

   bestDNA.load("bestDNA.save");

   arma::mat DNAS=1 - 2*arma::randu<arma::mat>(fitness.n_elem, inpWeights.n_elem + weights.n_elem + outWeights.n_elem + inpBiases.n_elem + biases.n_elem + outBiases.n_elem);
   DNAS=0.0000005*DNAS;
//   DNAS.load("DNAS21.save");
   for(int i=0; i < fitness.n_elem; i++){
       DNAS.row(i) = bestDNA.t();
   }
//   DNAS.load("DNASPure.save");
   arma::mat outputsNN = arma::zeros<arma::mat>(fitness.n_elem,outBiases.n_elem);

    QApplication a(argc, argv);
    Game game;
    double punishMove=0;

    // Use the following kind of loop to play advance the game manually with custom input
    // The a.exit closes the application
    std::vector<float> GUIinput = {0, 1};
//        if(i % 100 < 50){input = {0,1};}else{input={1,0};}// 133/44000
 //       QApplication::processEvents(QEventLoop::AllEvents, 1000);
    std::vector<float> GUIoutput = game.requestPosVel(GUIinput);

    for(int gen=0; gen < generations; gen++){
        fitness = 30000*ones<vec>(fitness.n_elem);
      for(int candidate=0; candidate < population; candidate++){
          punishMove = 0;
        for (int step = 0; step < 10000; step++){
            for(int j = 0; j < inpNN.n_elem; j++){
                inpNN(j) = GUIoutput[j];
            }
            outputsNN.row(candidate)= simpleNNvec(DNAS.row(candidate).t(),inpNN, inpWeights, weights, outWeights, inpBiases, biases, outBiases).t();
//            outputsNN.print("outpyt");
            GUIinput[0] = outputsNN(candidate,0);
            GUIinput[1] = outputsNN(candidate,1);
            GUIoutput = game.requestPosVel(GUIinput);

            if(gen % 2 == 0) GUIoutput[6] = 0;
            //Fitness Control
//            fitness(candidate) += 0.1*(GUIoutput[4] - GUIoutput[1])*(GUIoutput[4] - GUIoutput[1]);
            if(inputScore - GUIoutput[8] !=0 ){inputScore = GUIoutput[8]; fitness(candidate) -= 200;}
            if(enemyScore - GUIoutput[9] !=0 ){enemyScore = GUIoutput[9]; fitness(candidate) += 150;}
//            if(GUIoutput[5] !=0 ){fitness(candidate) += 0.01;}
//            if(GUIoutput[0] > 0 && abs(GUIoutput[4]) < 5){punishMove -= 0.10;}

            QApplication::processEvents(QEventLoop::AllEvents, 1000);
//        QThread::sleep(0);
        }
            fitness(candidate) += punishMove;
//            cout << "punish Move: " << punishMove << endl;
      }
            qDebug() << GUIoutput;
//        fitness.t().print("fit");
      DNAS = ga_eval(DNAS, 0.00001*fitness, 100.0,0.00001, temperature);

//      DNAS.save("DNASPure.save",raw_ascii);
      bestDNA = DNAS.row(index_min(fitness)).t();

//      DNAS.save("DNAS22.save",raw_ascii);
//      bestDNA.save("bestDNA.save",raw_ascii);
//      DNAS.save("DNASdevNull.save",raw_ascii);

        cout << "MixFitIs: " << max(fitness) << "\n";
        cout << "MinFitIs: " << min(fitness) << "\n";
        temperature -= 10*temperature/(double)generations;
        cout << "TempIs: " << temperature << endl;
        cout << "genis" << gen << "\n";
    }
    //    a.exit();

    // Use these lines to manullla play the game
//    game.loop();
//    a.exec();

}
