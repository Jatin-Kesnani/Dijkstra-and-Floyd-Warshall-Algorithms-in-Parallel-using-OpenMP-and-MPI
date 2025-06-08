#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <math.h> 
#include <sys/time.h> 
#include <mpi.h>
#include <limits.h>
#include <time.h> 
 
#define N 1000
#define SOURCE 1 
#define MAXINT INT_MAX 
 
void SingleSource(int n, int source, int *wgt, int *lengths, MPI_Comm comm){ 	
 	int i, j; 
 	int NV_Local; 			
 	int *Vertex_Marker; 		
 	int First_Vertex; 	 	
 	int Last_Vertex; 	
 	int u;
 	int Distance_U; 
	int Min_Pair_L[2], Min_Pair_G[2]; 
	int N_Processors, My_Rank;
	 
	MPI_Status status; 
	MPI_Comm_size(comm, &N_Processors); 
	MPI_Comm_rank(comm, &My_Rank); 
	 	
	NV_Local = n / N_Processors;  	
	First_Vertex = My_Rank*NV_Local;  	
	Last_Vertex = First_Vertex + NV_Local - 1; 
	
 	for(j=0 ; j<NV_Local ; j++){  	 	
 		lengths[j] = wgt[source*NV_Local + j]; 
 	} 
	
 	Vertex_Marker = (int *)malloc(NV_Local*sizeof(int));  	
 	for(j=0 ; j<NV_Local ; j++){ 
 	 	Vertex_Marker[j] = 1; 
 	} 
	
 	if(source >= First_Vertex && source <= Last_Vertex){  	 	
 		Vertex_Marker[source - First_Vertex] = 0; 
 	} 

	for(i=1 ; i<n ; i++){ 
 		Min_Pair_L[0] = MAXINT; 	
 		Min_Pair_L[1] = -1;  	 	
 		for(j=0 ; j<NV_Local ; j++){  	 	 	
 			if(Vertex_Marker[j] && lengths[j] < Min_Pair_L[0]){ 
 	 	 	 	Min_Pair_L[0] = lengths[j];  	 	 	 	
 	 	 	 	Min_Pair_L[1] = First_Vertex + j; 
 	 	 	} 
 	 	} 
 	 	
 	 	MPI_Allreduce(Min_Pair_L, Min_Pair_G, 1, MPI_2INT, MPI_MINLOC, comm);  	 	
 	 	Distance_U = Min_Pair_G[0];  	 		
 	 	u = Min_Pair_G[1]; 
 	 	 	
 	 	if(u == Min_Pair_L[1]){  	 	 	
 	 		Vertex_Marker[u - First_Vertex] = 0; 
 	 	} 
 	
 	 	for(j=0 ; j<NV_Local ; j++){  	 	 	
 	 		if(Vertex_Marker[j] && ((Distance_U + wgt[u*NV_Local + j]) <lengths[j])){  	 	 	
 	 			lengths[j] = Distance_U + wgt[u*NV_Local + j]; 
	 	 	} 
	 	} 
	} 
	free(Vertex_Marker); 
} 
 
int main(int argc, char *argv[]){      
	int N_Processors, My_Rank, NV_Local;     
	int **weight = calloc(N, sizeof(int*));    
	int *distance = calloc(N, sizeof(int*));  	  
	int *Local_WA; 				    
	int *Local_DV; 				    
	int *Send_Buf = calloc(N*N, sizeof(int));  	
	int i, j, k;         
	double time_start, time_end;     
	struct timeval tv;     
	struct timezone tz; 
     	
     	for(i=0 ; i<N ; i++){
 		weight[i] = calloc(N, sizeof(int));
 	}
     	
    	gettimeofday(&tv, &tz);     
    	time_start = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.00; 
     
   	 MPI_Init(&argc, &argv);     
    	 MPI_Comm_size(MPI_COMM_WORLD, &N_Processors); 
   	 MPI_Comm_rank(MPI_COMM_WORLD, &My_Rank); 
 
    	 NV_Local = N/N_Processors; 
 
   
        Local_WA = (int *)malloc(NV_Local*N*sizeof(int));     
    	Local_DV = (int *)malloc(NV_Local*sizeof(int)); 

    	if (My_Rank == SOURCE) {    	   	 
	 	for(i=0 ; i<N ; i++){     	 	   	  
	 		for(j=i+1 ; j<N ; j++){     	 	 	
	 			weight[i][j] = 250+rand() % 100;
	 	 	} 	 	
	 	 	for(k=0 ; k<N_Processors ; ++k){  	 	 	
	 	 		for(i=0 ; i<N ;++i){  	 	 	 	
	 	 			for(j=0 ; j<NV_Local ; ++j){  	 	 	 	 	
	 	 				Send_Buf[k*N*NV_Local+i*NV_Local+j]=weight[i][k*NV_Local+j]; 
	 	 	 	 	} 
	 	 	 	} 
	 	 	} 
	    	}
    	} 
    	MPI_Scatter(Send_Buf, NV_Local*N, MPI_INT, Local_WA, NV_Local*N, MPI_INT, SOURCE, MPI_COMM_WORLD);      
    	SingleSource(N, SOURCE, Local_WA, Local_DV, MPI_COMM_WORLD); 
    	MPI_Gather(Local_DV, NV_Local, MPI_INT, distance, NV_Local, MPI_INT, SOURCE, MPI_COMM_WORLD); 
    
    	if(My_Rank == SOURCE){
 	 	//printf("\nThe Distance Vector is :\n"); 
	 	//for(i=0 ; i<N ; i++){ 
	 	// 	printf("Source [%d] -> Vertex [%d] || Distance = %d\n", SOURCE, i, distance[i]);
	 	//} 
 	   	
 	   	printf("\n");
 	   	printf("Number of Vertices : %d\n", N); 
 	 	
 	 	gettimeofday(&tv, &tz); 
 	 	time_end = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.00;  	 	
 	 	
 	 	printf("Time Cost is       : %lf\n", time_end - time_start);  	
 		printf("\n");
    	}
    	 
    	free(Local_WA);      
    	free(Local_DV);     
    	MPI_Finalize();     
    	
    	return 0; 
}
