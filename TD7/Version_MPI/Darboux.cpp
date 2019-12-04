#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <omp.h>
#include "Terrain.h"
#include "Point.h"
#include <mpi.h>
#define ROOT 0
using namespace std;

void lecture(FILE *f, float *tab, int nr, int nc) {
    int iter = 0;
    for (int i = 0; i < nr; i++)
        for (int j = 0; j < nc; j++) {
            fscanf (f, "%f", &(tab[i * nc + j]));
            iter++;
        }
}
void ecriture(float* tab, int nr, int nc, float* Z)
{
    for (int i = 0; i < nr; i++)
        for (int j = 0; j < nc; j++)
            Z[i*nc+j] = tab[i*nc+j];
}

mnt* Lecture_Terrain(char *nom, int root) {
    int pid, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    mnt* terrain = new mnt();
    FILE *f;
    int* n = new int[2];
    if (pid == root) {
        f = fopen(nom, "r");
        if (f != NULL) {
            fscanf(f, "%d", n+1);
            fscanf(f, "%d", n);
        }
    }
    MPI_Bcast(n, 2, MPI_2INT, root, MPI_COMM_WORLD);
    terrain->set_t(n[0],n[1]);



    int nrow_local = n[0] / nprocs;
    int reste = n[0] % nprocs;
    if (pid < reste)
        nrow_local++;

    int debut = (pid==0) ? 0 : 1;
    terrain->set_tlocal(nrow_local,n[1],debut);

    if (pid == 0 || pid == nprocs - 1)
        terrain->set_ZW(true);
     else
        terrain->set_ZW(false);

    float novalue;
    if (pid == root) {
        int tmp;
        fscanf(f, "%d", &tmp);
        fscanf(f, "%d", &tmp);
        fscanf(f, "%d", &tmp);
        fscanf(f, "%f", &novalue);
    }


    MPI_Bcast(&novalue, 1, MPI_FLOAT, root, MPI_COMM_WORLD);

    terrain->set_novalue(novalue);


    if (pid == root) {
        int taille = n[0] / nprocs;
        int nrow_alire;
        float *tab;
        for (int i = 0; i < nprocs; i++) {
            nrow_alire = (i < reste) ? taille + 1 : taille;
            tab = new float[(nrow_alire) * n[1]];
            lecture (f, tab, nrow_alire, n[1]);
            if (i != pid)
                MPI_Ssend (tab, nrow_alire * n[1], MPI_FLOAT, i, TAG, MPI_COMM_WORLD);
            else {
                if (pid == 0)
                    ecriture (tab, nrow_alire, n[1], terrain->get_Z_ptr ());
                else
                    ecriture (tab, nrow_alire, n[1], terrain->get_Z_ptr () + n[1]);
            }
            delete tab;
        }
    } else {
        if (pid == 0)
            MPI_Recv (terrain->get_Z_ptr (), nrow_local * n[1], MPI_FLOAT, root, TAG, MPI_COMM_WORLD,
                      MPI_STATUS_IGNORE);

        else
            MPI_Recv (terrain->get_Z_ptr () + n[1], nrow_local * n[1], MPI_FLOAT, root, TAG, MPI_COMM_WORLD,
                      MPI_STATUS_IGNORE);
        }

    if (pid == root)
        if (f != NULL)
            fclose(f);

    delete[] n;
    return terrain;
}

bool Remplissage(mnt *terrain) {//HERE

  bool modification = false;

  float *Z = terrain->get_Z_ptr();
  float *W = terrain->get_W_ptr();

  float no_value = terrain->get_novalue();

  int nr = terrain->get_nr();
  int nc = terrain->get_nc();
  float *W_new = new float[nr * nc];
  //#pragma omp parallel for
  for (int i = 0; i < nr; i++) {
    //#pragma omp parallel for
    for (int j = 0; j < nc; j++) {
      Point *centre = new Point(i, j);
      float Zc = Z[i * nc + j];
      float Wc = W[i * nc + j];
      float Wn;
      W_new[i * nc + j] = W[i * nc + j];
      if (Wc > Zc && Wc != no_value) {
        Point *v1 = new Point(i, j + 1);
        for (int k = 0; k < 8; k++) {
          float Epsilon = 1.0;
          Wn = W[v1->r * nc + v1->c];
          if (Zc >= Wn + Epsilon) {
            W_new[i * nc + j] = Zc;
            modification = true;
          } else if (Wc > Wn + Epsilon) {
            W_new[i * nc + j] = Wn + Epsilon;
            modification = true;
          }
          if (k != 7) {
            Point *v2 = v1->Next_Voisin(centre);
            v1->r = v2->r;
            v1->c = v2->c;
            delete v2;
          }
        }
        delete v1;
      }
      delete centre;
    }
  }
  //#pragma omp parallel for
  for (int i = 0; i < nr; i++)
  for (int j = 0; j < nc; j++)
  W[i * nc + j] = W_new[i * nc + j];

  free(W_new);

  return modification;
}
