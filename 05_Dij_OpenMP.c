#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <sys/time.h> 
#include <time.h>
#include <omp.h> 
#include <limits.h> 
 
#define N 5000
#define SOURCE 1 
#define MAXINT INT_MAX
 
void dijkstra(int** graph, int source){ 
 	int *visited = calloc(N, sizeof(int*));
 	int *distance = calloc(N, sizeof(int*));
 	int First_Vertex; 				
 	int T_ID; 				
 	int Last_Vertex; 					
 	int Local_MD; 					
 	int Local_MV; 				 	
 	int Step_Count;  	
 	int nth, i, md, mv;

	for(i=0 ; i<N ; i++){ 
		visited[i] = 0; 
	 	distance[i] = graph[source][i]; 
	} 
	visited[source] = 1; 
 	 
 	# pragma omp parallel private ( First_Vertex, T_ID, Last_Vertex, Local_MD, Local_MV, Step_Count )  	shared ( visited, md, distance, mv, nth, graph ) 
 	{ 
 	 	T_ID = omp_get_thread_num ( );  	 	
 	 	nth = omp_get_num_threads ( );   	 	
 	 	First_Vertex =   (T_ID * N ) / nth;  	 	
 	 	Last_Vertex  =   ((T_ID + 1) * N) / nth - 1; 
	 	
 	 	for(Step_Count=1 ; Step_Count<N ; Step_Count++) { 
 	 	 	# pragma omp single  
 	 	 	{
 	 	 	 	md = MAXINT;
 	 	 	 	mv = -1;  
 	 	 	} 
 	 	 	int k; 
 	 		Local_MD = MAXINT;  	
 	 		Local_MV = -1; 
 
 	 	 	for(k=First_Vertex ; k<=Last_Vertex ; k++){  	 	 	 	
 	 	 		if(!visited[k] && distance[k] < Local_MD){ 
 	 	 	 	 	Local_MD = distance[k]; 
 	 	 	 	 	Local_MV = k; 
 	 	 	 	}   
 	 	 	} 

 	 	 	# pragma omp critical 
 	 	 	{ 
 	 	 	 	if(Local_MD < md){ 
 	 	 	 	 	md = Local_MD;  	 	 	 	 	
 	 	 	 	 	mv = Local_MV; 
 	 	 	 	} 
 	 	 	} 

	 	 	# pragma omp barrier 
 	 
			# pragma omp single  
	 	 	{ 	 
 	 	 	 	if(mv != - 1){  	 	 	 	 	
 	 	 	 		visited[mv] = 1; 
 	 	 	 	} 
 	 	 	} 
 	 	 	# pragma omp barrier  
 	 	 	 
 	 	 	if(mv != -1){ 
 	 	 	 	int j; 
 	 	 	 	for(j=First_Vertex ; j<=Last_Vertex ; j++){  	 	 	 	 	
 	 	 	 		if(!visited[j] && graph[mv][j] < MAXINT && distance[mv] + graph[mv][j] < distance[j]){ 
 	 	 	 	 	 	distance[j] = distance[mv] + graph[mv][j]; 
 	 	 	 	 	}
 	 	 	 	} 
 	 	 	} 
 	 	 	#pragma omp barrier 
 	 	} 
 	} 
	//printf("\nThe Distance Vector is :\n"); 
 	//for(i=0 ; i<N ; i++){ 
 	// 	printf("Source [%d] -> Vertex [%d] || Distance = %d\n", source, i, distance[i]);
 	//} 
}  

int main(int argc, char *argv[]){
 	int i, j;
 	int** graph = calloc(N, sizeof(int*)); 
 	
 	for(int i = 0; i < N; i++){
 		graph[i] = calloc(N, sizeof(int));
 	}
 	 	
 	int threads = atoi(argv[1]);
 	//printf("Please Enter Number of Threads : "); 
 	//scanf("%d", &threads); 
 	
 	omp_set_num_threads(threads); 
 	
 	double time_start, time_end; 
	struct timeval tv; 
	struct timezone tz; 
	
	gettimeofday(&tv, &tz);  	
	time_start = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.00; 
 	
 	srand(time(0));
 	for(i=0 ; i<N ; i++){  	 	
 		for(j=i+1 ; j<N ; j++){  	 	 	
 			graph[i][j] = 250+rand() % 100;
			graph[j][i] = graph[i][j]; 
 	 	 }
 	} 
	dijkstra(graph, SOURCE);  
 
	gettimeofday(&tv, &tz); 
	time_end = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.00; 
	
	printf("\n");  	
 	printf("Number of Vertices : %d\n", N);
 	
	printf("Time Cost is       : %lf\n", time_end - time_start);  	
 	printf("\n");
 	 
 	return 0; 
}
