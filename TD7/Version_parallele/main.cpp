#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <chrono>
#include <omp.h>
#include "Terrain.h"
#include "Darboux.h"

#define BENCHMARK(fun)							\
do { chrono::time_point<chrono::system_clock> start, end;		\
  start = chrono::system_clock::now();				\
  fun;								\
  end = chrono::system_clock::now();					\
  chrono::duration<double> elapsed_seconds = end-start;		\
  time_t end_time = chrono::system_clock::to_time_t(end);		\
  cout << #fun 							\
  << " elapsed time: " << elapsed_seconds.count() << "s\n";	\
} while (0)

using namespace std;

int main(int argc, char **argv) {
    char *nom = argv[1];
    int nthreadsds = atoi(argv[2]);

    mnt *terrain = Lecture_Terrain(nom);
      terrain->printZ();

    terrain->set_max();


      cout << "max=" << terrain->get_max() <<"\n"<< "novalue=" << terrain->get_novalue() << endl;
    Init_W(terrain);
    terrain->printW();
    bool modification = true;

      while (modification)
        modification = Remplissage(terrain);

    terrain->printW();

     for (int i=0; i<terrain->get_nr(); i++) {
         for (int j = 0; j < terrain->get_nc(); j++)
             cout << terrain->get_Z(i, j) - terrain->get_W(i, j) << " ";
            cout << endl;
     }

    delete terrain;
    return 0;
}
