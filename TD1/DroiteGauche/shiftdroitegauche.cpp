#include <mpi.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

void shift_droite_gauche_circulaire(int n, int* in, int* out)
{
  int pid, nprocs;
  MPI_Comm_rank(MPI_COMM_WORLD, &pid );
  MPI_Comm_size (MPI_COMM_WORLD, &nprocs );
  MPI_Status status;
  int proc_droite=(pid+1)%nprocs;
  int proc_gauche=((pid-1)+nprocs)%nprocs;


  int moitiee=n/2;
  int *d = new int[moitiee];
  int *g = new int[moitiee];
  int *d_f = new int[moitiee];
  int *g_f = new int[moitiee];

  for (size_t i = 0; i < n; i++) {
    if (i<moitiee){
        g[i]=in[i];
    }else{
      d[i - moitiee]=in[i];
    }
  }

  if (pid%2==0 ){
    MPI_Recv(d_f,moitiee,MPI_INT,proc_droite,42,MPI_COMM_WORLD,&status);
    MPI_Send(d,moitiee,MPI_INT,proc_droite,42,MPI_COMM_WORLD);
    MPI_Send(g,moitiee,MPI_INT,proc_gauche,42,MPI_COMM_WORLD);
    MPI_Recv(g_f,moitiee,MPI_INT,proc_gauche,42,MPI_COMM_WORLD,&status);
    for (size_t i = 0; i < n; i++) {
      if (i<moitiee){
          out[i]=g[i];
      }else{
        out[i] = d[i - moitiee];
      }
    }

  }
  else {
    MPI_Recv(d_f,moitiee,MPI_INT,proc_droite,42,MPI_COMM_WORLD,&status);
    MPI_Send(d,moitiee,MPI_INT,proc_droite,42,MPI_COMM_WORLD);
    MPI_Send(g,moitiee,MPI_INT,proc_gauche,42,MPI_COMM_WORLD);
    MPI_Recv(g_f,moitiee,MPI_INT,proc_gauche,42,MPI_COMM_WORLD,&status);
    for (size_t i = 0; i < n; i++) {
      if (i<moitiee){
          out[i]=g[i];
      }else{
        out[i] = d[i - moitiee];
      }
    }


  }

  if (pid==0){
    //MPI_Recv(&a_local,1,MPI_INT,((pid-1)+nprocs)%nprocs,34,MPI_COMM_WORLD,&status);
    MPI_Recv(out,n,MPI_INT,MPI_ANY_SOURCE,42,MPI_COMM_WORLD,&status);
    MPI_Recv(out,n,MPI_INT,MPI_ANY_SOURCE,42,MPI_COMM_WORLD,&status);
  }

}

void shift_droite_gauche(int n, int* in, int* out)
{

}

int main ( int argc , char **argv )
{
  int pid, nprocs;
  MPI_Status status;
  MPI_Init (&argc , &argv) ;
  MPI_Comm_rank(MPI_COMM_WORLD, &pid ) ;
  MPI_Comm_size (MPI_COMM_WORLD, &nprocs ) ;

  int nlocal = 2*atoi(argv[1]);
  int* tablocal = new int[nlocal];
  int* tabres = new int[nlocal];

  srand(time(NULL)+pid);
  for (int i=0; i<nlocal; i++)
    tablocal[i] = rand()%10;

  cout << "je suis " << pid << " et j'ai le tableau initial : ";
  for (int i=0; i<nlocal; i++)
    cout << tablocal[i] << " ";
  cout << endl;

  shift_droite_gauche(nlocal,tablocal,tabres);

//  shift_droite_gauche(nlocal,tablocal,tabres);

  cout << "je suis " << pid << " et j'ai le tableau final : ";
  for (int i=0; i<nlocal; i++)
    cout << tabres[i] << " ";
  cout << endl;

  delete[] tablocal;
  delete[] tabres;
  MPI_Finalize() ;
  return 0 ;
}
