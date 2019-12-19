#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "mpi.h"

int main( int argc, char** argv ) 
{
	int rank, size;
        double starttime, endtime;
	
	char hostname[1024];
        gethostname(hostname, 1024);
    
        starttime = MPI_Wtime();
        
	MPI_Init( &argc, &argv );  
        
 	MPI_Comm_size( MPI_COMM_WORLD, &size ); 
	MPI_Comm_rank( MPI_COMM_WORLD, &rank ); 
        
	printf( "Hello world from %s process %d of %d\n", hostname, rank, size ); 

        endtime = MPI_Wtime();
       
        printf("Total time taken by cluster: %f\n", endtime - starttime  );
        
	MPI_Finalize();

        

 
	
	return 0;
 } 
