  #include <mpi.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <time.h>
  #include <iostream>

  using namespace std;

  void calcul_max(int n, int* tab, int root, int* maxtab)
  {

    int pid, nprocs;
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &pid ) ;
    MPI_Comm_size (MPI_COMM_WORLD, &nprocs ) ;



    maxtab[0]=tab[0];
    maxtab[1]=0;
    for(int i=0;  i<n ;i++){
      if(maxtab[0]<tab[i]){
        maxtab[0]=tab[i];
        maxtab[1]=i+pid*n;
      }
    }
    if (pid!=root){
      MPI_Send(maxtab,2,MPI_INT,root,42,MPI_COMM_WORLD);
    }

    if(pid==root){

      for(int proc=1;proc<nprocs;proc++){
        int *maxtab_tmp = new int[2];
        MPI_Recv(maxtab_tmp,2,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
        if (maxtab_tmp[0] > maxtab[0]) {
          maxtab[0]=maxtab_tmp[0];
          maxtab[1]=maxtab_tmp[1];

        }
      }
    }

  }

  int main ( int argc , char **argv )
  {
    int pid, nprocs;
    MPI_Status status;
    MPI_Init (&argc , &argv) ;
    MPI_Comm_rank(MPI_COMM_WORLD, &pid ) ;
    MPI_Comm_size (MPI_COMM_WORLD, &nprocs ) ;

    int nlocal = atoi(argv[1]);
    int root = atoi(argv[2]);

    int* tablocal = new int[nlocal];


    srand(time(NULL)+pid);
    for (int i=0; i<nlocal; i++)
    tablocal[i] = rand()%100;

    cout << "je suis " << pid << " et j'ai le tableau local : ";
    for (int i=0; i<nlocal; i++)
    cout << tablocal[i] << " ";
    cout << endl;

    int* maxtab = new int[2];
    calcul_max(nlocal,tablocal,root,maxtab);

    if (pid==root) {
      cout << "le max est : " << maxtab[0] << endl;
      cout << "c'est l'élément " << maxtab[1] << " du tableau" << endl;
    }

    delete[] tablocal;
    delete[] maxtab;
    MPI_Finalize() ;
    return 0 ;
  }
