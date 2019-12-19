/* ************************************************************* */
/*                                                               */
/* mat_mat.c                                                     */
/*                                                               */
/* Created by Andrew Pineda and John Grondalski 8/18/00          */
/*                                                               */
/* This program computes, in parallel, the matrix-matrix product */
/*                                                               */
/*      A=B*C                                                    */
/* ************************************************************* */

#include <stdio.h>
#include <mpi.h>

#define NCOLS 4
#define NROWS 4

int main(int argc, char **argv) {

  double t1, t2; 
  t1 = MPI_Wtime();
  
  int i,j,k,l;

  int ierr, rank, size, root;

  float A[NROWS][NCOLS];
  float Apart[NCOLS];
  float Bpart[NCOLS];
  float C[NROWS][NCOLS];
  float A_exact[NROWS][NCOLS];
  float B[NCOLS][NCOLS];

  root = 0;

  /* Initiate MPI. */

  ierr=MPI_Init(&argc, &argv);
  ierr=MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  ierr=MPI_Comm_size(MPI_COMM_WORLD, &size);

  /* Initialize B and C. */

  if (rank == root) {

    for(j=0;j<NCOLS;j++) {
      for(k=0;k<NCOLS;k++) {
	  B[j][k]=(float)(j+k);
	}
    }
    for(j=0;j<NROWS;j++) {
      for(k=0;k<NCOLS;k++) {
	  C[j][k]=(float)(10*j+k);
	}
    }

  }

  /* Put up a barrier until I/O is complete */

  ierr=MPI_Barrier(MPI_COMM_WORLD);

  /* Scatter matrix B by rows. */

  ierr=MPI_Scatter(B,NCOLS,MPI_FLOAT,Bpart,NCOLS,MPI_FLOAT,root,MPI_COMM_WORLD);

  /* Broadcast C */

  ierr=MPI_Bcast(C,NROWS*NCOLS,MPI_FLOAT,root,MPI_COMM_WORLD);

  /* Do the vector-scalar multiplication. */

  for(j=0;j<NCOLS;j++){
    Apart[j] = 0.0;
    for(k=0; k<NROWS; k++)
      Apart[j] += Bpart[k]*C[k][j];	  
  }

  /* Gather matrix A. */

  ierr=MPI_Gather(Apart,NCOLS,MPI_FLOAT,A,NCOLS,MPI_FLOAT, root,MPI_COMM_WORLD);

  /* Report results */

  if (rank == 0) {

    printf("\nThis is the result of the parallel computation:\n\n");

    for(j=0;j<NROWS;j++) {
      for(k=0;k<NCOLS;k++) {
	printf("A[%d][%d]=%g\n",j,k,A[j][k]);
      }
    }

    for(j=0;j<NROWS;j++) {
      for(k=0;k<NCOLS;k++) {
	A_exact[j][k] = 0.0;
	for(l=0;l<NCOLS;l++) {
	  A_exact[j][k] += B[j][l]*C[l][k];	  
	}
      }
    }
		
    printf("\nThis is the result of the serial computation:\n\n");
    for(j=0;j<NROWS;j++) {
      for(k=0;k<NCOLS;k++) {
	printf("A_exact[%d][%d]=%g\n",j,k,A_exact[j][k]);
      }
    }
  }
   
  t2 = MPI_Wtime(); 
  printf( "Total time taken by cluster: %f\n", t2 - t1 );  
  MPI_Finalize();
   
}