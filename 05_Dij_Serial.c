#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/time.h>
#include <time.h>
#include <limits.h> 
 
#define N 5000
#define SOURCE 1 
#define MAXINT INT_MAX 

void dijkstra(int** graph, int source){ 
 	int distance[N];	
 	int visited[N];  	
 	int i, j, count, NextVertex, MinWeight; 
	
 	for(i=0 ; i<N ; i++){ 
		distance[i] = graph[source][i];
		visited[i] = 0; 
	}

 	visited[source] = 1;
	count = 1;	
 	while(count < N){ 
 	 	MinWeight = MAXINT; 	 	
 	 	for(i=0 ; i<N ; i++){  	 	 	
 	 		if (distance[i] < MinWeight && !visited[i]){ 
 	 	 	 	MinWeight = distance[i]; 
 	 	 	 	NextVertex = i; 
 	 	 	} 
 	 	}
 	 	visited[NextVertex] = 1;  
 	 	count++;	
	 	for(i = 0; i < N; i++){ 
	 		if(!visited[i] && MinWeight + graph[NextVertex][i] < distance[i]){
	 			distance[i] = MinWeight + graph[NextVertex][i]; 
		 	 } 
	 	 } 
 	} 
 	 
 	//printf("\nThe Distance Vector is :\n"); 
 	//for(i=0 ; i<N ; i++){ 
 	// 	printf("Source [%d] -> Vertex [%d] || Distance = %d\n", source, i, distance[i]);
 	//}  
}
 
int main(){ 
	int** weight = calloc(N, sizeof(int*));
	int i, j;
	for(i=0 ; i<N ; i++){
		weight[i] = calloc(N, sizeof(int));
	}
	
	
 	double time_start, time_end;
 	struct timeval tv;
 	struct timezone tz;
 	
 	gettimeofday(&tv, &tz);
 	time_start = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.00;
	
	srand(time(0)); 	
 	for(i=0 ; i<N ; i++){  	 	
 		for(j=i+1 ; j<N ; j++){
 			weight[i][j] = 250+rand()%100;
			weight[j][i] = weight[i][j];
		}  
	} 
 	
 	dijkstra(weight, SOURCE);  	
 	
 	printf("\n");  	
 	printf("Number of Vertices : %d\n", N);  	
 	
 	gettimeofday(&tv, &tz);  	
 	time_end = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.00;  	
 	
 	printf("Time Cost is       : %lf\n", time_end - time_start);  	
 	printf("\n");  	
 	
 	return 0; 
}
