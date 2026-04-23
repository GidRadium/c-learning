#pragma once

#include <stddef.h>

typedef struct Graph Graph;

typedef  struct PartitionEntry {
    int city;
    int state;
} PartitionEntry;

typedef struct Partition {
    PartitionEntry* data;
    size_t size;
} Partition;

Graph* graphCreate(int n, int m);

void graphAddEdge(Graph* g, int u, int v, int length);

void graphDelete(Graph** g);

Partition graphSolve(Graph* g, int capitalsCount, const int* capitals);

void partitionDelete(Partition* a);
