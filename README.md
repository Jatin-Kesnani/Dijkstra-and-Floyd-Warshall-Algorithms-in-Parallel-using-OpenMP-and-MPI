# Dijkstra-and-Floyd-Warshall-Algorithms-in-Parallel-using-OpenMP-and-MPI

Implementation of parallelized Dijkstra's Single-Source Shortest Path and Floyd-Warshall All-Pairs Shortest Path algorithms using OpenMP and MPI.

## Features
- **OpenMP Implementations**:
  - Dijkstra's algorithm with hybrid data/task decomposition
  - Floyd-Warshall algorithm with dynamic scheduling
- **MPI Implementations**:
  - Dijkstra's algorithm using MPI Scatter/Gather
  - Collective communication with MPI Allreduce
- **Performance Benchmarking**:
  - Sequential vs parallel execution time comparison
  - Scalability testing with large graphs (up to 9500 nodes)

## Technologies
- C/C++ programming
- OpenMP (v4.5+)
- MPI (OpenMPI v4.0+)
- Linux environment

## Installation
```bash
# Clone repository
git clone https://github.com/Jatin-Kesnani/Dijkstra-and-Floyd-Warshall-Algorithms-in-Parallel-using-OpenMP-and-MPI.git
```
