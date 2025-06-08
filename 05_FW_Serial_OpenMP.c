#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define N 500

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

int distance_matrix[N][N] = {0};

int main(int argc, char *argv[]){
	int N_Threads;
  	int SRC_Iterator, DST_Iterator, MID_Iterator;
  
  	for(SRC_Iterator=0 ; SRC_Iterator<N ; SRC_Iterator++){
    		for(DST_Iterator=0 ; DST_Iterator<N ; DST_Iterator++){
      			if(SRC_Iterator != DST_Iterator){
        			distance_matrix[SRC_Iterator][DST_Iterator] = rand() % 20;
      			}
    		}
  	}
  
  	double start_time = omp_get_wtime();
  
  	for(MID_Iterator=0 ; MID_Iterator<N ; MID_Iterator++){
    		int * dm = distance_matrix[MID_Iterator];
    		for(SRC_Iterator=0 ; SRC_Iterator<N ; SRC_Iterator++){
      			int * ds=distance_matrix[SRC_Iterator];
      			for(DST_Iterator=0 ; DST_Iterator<N ; DST_Iterator++){
        			ds[DST_Iterator]=min(ds[DST_Iterator],ds[MID_Iterator]+dm[DST_Iterator]);
      			}
    		}
  	}
  
  	double time = omp_get_wtime() - start_time;
  	printf("*** Sequential Execution ***\n");
  	printf("Total Time (in sec) : %.2f\n", time);
  	{
  		printf("\n\n*** Parallel Execution ***\n");
	  	int N_Threads = 0;
		printf("ENTER NO OF THREADS : ");
		scanf("%d", &N_Threads);
	    	omp_set_num_threads(N_Threads);
	    
	    	double start_time = omp_get_wtime();
	    
	    	#pragma omp parallel shared(distance_matrix)
    		for(MID_Iterator=0 ; MID_Iterator<N ; MID_Iterator++){
	      		int * dm=distance_matrix[MID_Iterator];
	      		#pragma omp parallel for private(SRC_Iterator, DST_Iterator) schedule(dynamic)
      			for(SRC_Iterator=0 ; SRC_Iterator<N ; SRC_Iterator++){
        			int * ds=distance_matrix[SRC_Iterator];
        			for(DST_Iterator=0 ; DST_Iterator<N ; DST_Iterator++){
          				ds[DST_Iterator]=min(ds[DST_Iterator],ds[MID_Iterator]+dm[DST_Iterator]);
        			}
      			}
    		}
    
	    	double time = omp_get_wtime() - start_time;
	    	printf("Total Time for [%d] Threads (in sec) : %.2f\n", N_Threads, time);
    	}
  	
  	return 0; 
}
