#pragma once

#include <stddef.h>

// Data struct for V and E with weigths (length).
typedef struct Graph Graph;

// Contains entry of answer of problem.
typedef struct {
    int city;
    int state;
} PartitionEntry;

// Contains answer of problem.
typedef struct {
    PartitionEntry* data;
    size_t size;
} Partition;

// Allocates memory for graph.
Graph* graphCreate(int numberOfVertices, int numberOfEdges);

// Allocates memory for two edges.
void graphAddEdge(Graph* graph, int u, int v, int length);

// Frees all memory allocated by graphCreate and graphAddEdge.
void graphDelete(Graph** graph);

// Solves the problem.
Partition graphSolve(Graph* graph, int numberOfCapitals, const int* capitals);

// Frees answer struct.
void partitionDelete(Partition* partition);
