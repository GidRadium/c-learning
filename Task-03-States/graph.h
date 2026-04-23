#pragma once

#include <stddef.h>

typedef struct Graph Graph;

typedef struct {
    int city;
    int state;
} PartitionEntry;

typedef struct {
    PartitionEntry* data;
    size_t size;
} Partition;

Graph* graphCreate(int numberOfVertices, int numberOfEdges);

void graphAddEdge(Graph* graph, int u, int v, int length);

void graphDelete(Graph** graph);

Partition graphSolve(Graph* graph, int numberOfCapitals, const int* capitals);

void partitionDelete(Partition* partition);
