//#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstring>

#include <omp.h>



using namespace std;


int i4_min ( int i1, int i2 ){
    int value;

    if ( i1 < i2 )
    {
        value = i1;
    }
    else
    {
        value = i2;
    }
    return value;
}
//****************************************************************************80

void i4pp_delete ( int **a, int m, int n ){
    int i;

    for ( i = 0; i < m; i++ )
    {
        delete [] a[i];
    }

    delete [] a;

    return;
}
//****************************************************************************80

int **i4pp_new ( int m, int n ){
    int **a;
    int i;

    a = new int *[m];

    if ( a == NULL )
    {
        cerr << "\n";
        cerr << "I4PP_NEW - Fatal error!\n";
        cerr << "  Unable to allocate row pointer array.\n";
        exit ( 1 );
    }

    for ( i = 0; i < m; i++ )
    {
        a[i] = new int[n];
        if ( a[i] == NULL )
        {
            cerr << "\n";
            cerr << "I4PP_NEW - Fatal error!\n";
            cerr << "  Unable to allocate row array.\n";
            exit ( 1 );
        }
    }

    return a;
}

//****************************************************************************

int main ( int argc, char *argv[] ){

  int n_nodes = atoi(argv[1]);
  int m = 1000;
  int n = 1000;

  int **b;
  int c;
  int c_max;
  int count_max = 2000;
  int count = 0;
  int **g;
  int i;
  int ierror;
  int j;
  int jhi;
  int jlo;
  int k;
  string filename = "mandelbrot.ppm";
  ofstream output;
  int **r;
  double wtime;
  double wtime_total;
  double x_max =   1.25;
  double x_min = - 2.25;
  double x;
  double x1;
  double x2;
  double y_max =   1.75;
  double y_min = - 1.75;
  double y;
  double y1;
  double y2;

  b = i4pp_new ( m, n );
  g = i4pp_new ( m, n );
  r = i4pp_new ( m, n );

  cout << "\n";
  cout << "MANDELBROT_OPENMP\n";
  cout << "  C++/OpenMP version\n";
  cout << "\n";
  cout << "  Create an ASCII PPM image of the Mandelbrot set.\n";
  cout << "\n";
  cout << "  For each point C = X + i*Y\n";
  cout << "  with X range [" << x_min << "," << x_max << "]\n";
  cout << "  and  Y range [" << y_min << "," << y_max << "]\n";
  cout << "  carry out " << count_max << " iterations of the map\n";
  cout << "  Z(n+1) = Z(n)^2 + C.\n";
  cout << "  If the iterates stay bounded (norm less than 2)\n";
  cout << "  then C is taken to be a member of the set.\n";
  cout << "\n";
  cout << "  An ASCII PPM image of the set is created using\n";
  cout << "    M = " << m << " pixels in the X direction and\n";
  cout << "    N = " << n << " pixels in the Y direction.\n";

  wtime = omp_get_wtime ( );

  //***************** Partie à paralléliser
    //#pragma omp task
    #pragma omp parallel for private(x,y,x1,x2,y1,y2,c,i,j,k)
    for (i = 0; i < m; i++) {
        #pragma omp task
        //untied
        for (j = 0; j < n; j++) {
            x = ((double) (j - 1) * x_max + (double) (m - j) * x_min) / (double) (m - 1);
            y = ((double) (i - 1) * y_max + (double) (n - i) * y_min) / (double) (n - 1);
            count = 0;
            x1 = x;
            y1 = y;

            for (k = 1; k <= count_max; k++) {
                x2 = x1 * x1 - y1 * y1 + x;
                y2 = 2 * x1 * y1 + y;

                if (x2 < -2.0 || 2.0 < x2 || y2 < -2.0 || 2.0 < y2) {
                    count = k;
                    break;
                }
                x1 = x2;
                y1 = y2;
            }

            if ((count % 2) == 1) {
                r[i][j] = 255;
                g[i][j] = 255;
                b[i][j] = 255;
            } else {
                c = (int) (255.0 * sqrt(sqrt(sqrt(((double) (count) / (double) (count_max))))));
                r[i][j] = 3 * c / 5;
                g[i][j] = 3 * c / 5;
                b[i][j] = c;
            }
        }
    }


  wtime = omp_get_wtime ( ) - wtime;
  cout << "\n";
  cout << "  Time = " << wtime << " seconds.\n";


    //Write data to an ASCII PPM file.

  output.open ( filename.c_str ( ) );

  output << "P3\n";
  output << n << "  " << m << "\n";
  output << 255 << "\n";
  for ( i = 0; i < m; i++ ){
    for ( jlo = 0; jlo < n; jlo = jlo + 4 ){
      jhi = i4_min ( jlo + 4, n );
      for ( j = jlo; j < jhi; j++ ){
	output << "  " << r[i][j]
	       << "  " << g[i][j]
	       << "  " << b[i][j] << "\n";
      }
      output << "\n";
    }
  }

  output.close ( );
  cout << "\n";
  cout << "  Graphics data written to \"" << filename << "\".\n";

  //    Free memory.


  i4pp_delete ( b, m, n );
  i4pp_delete ( g, m, n );
  i4pp_delete ( r, m, n );

  //  Terminate.


  cout << "\n";
  cout << "MANDELBROT_OPENMP\n";
  cout << "  Normal end of execution.\n";
  cout << "\n";
//2.31489 seconds. seq
  return 0;
}
