#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sstream>
#include <fstream>
#include <iostream>

#include "Terrain.h"
#include "Darboux.h"

using namespace std;

int main(int argc, char **argv) {
    char *nom = argv[1];

    mnt *terrain = Lecture_Terrain(nom);
    //  terrain->printZ();

    terrain->set_max();

    //  cout << "max=" << terrain->get_max() << "novalue=" << terrain->get_novalue() << endl;
    Init_W(terrain);
    //   terrain->printW();
    bool modification = true;
    while (modification)
        modification = Remplissage(terrain);

    //   terrain->printW();

    /* for (int i=0; i<terrain->get_nr(); i++) {
         for (int j = 0; j < terrain->get_nc(); j++)
             cout << terrain->get_Z(i, j) - terrain->get_W(i, j) << " ";
            cout << endl;
     }*/

    delete terrain;


    return 0;
}
