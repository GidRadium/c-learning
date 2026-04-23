#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

int main()
{
    int n;
    int m;

    if (scanf("%d %d", &n, &m) != 2) {
        return 1;
    }

    Graph* graph = graphCreate(n, m);

    for (int i = 0; i < m; i++) {
        int u;
        int v;
        int length;

        scanf("%d %d %d", &u, &v, &length);
        graphAddEdge(graph, u, v, length);
    }

    int k;
    scanf("%d", &k);

    int* capitals = malloc(k * sizeof(int));

    for (int i = 0; i < k; i++) {
        scanf("%d", &capitals[i]);
    }

    Partition partition = graphSolve(graph, k, capitals);

    for (size_t i = 0; i < partition.size; i++) {
        printf("%d %d\n", partition.data[i].city, partition.data[i].state);
    }

    partitionDelete(&partition);
    graphDelete(&graph);
    free(capitals);

    return 0;
}
