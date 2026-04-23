#include "graph.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) {
        return -1;
    }

    Graph* graph = graphCreate(n, m);
    for (int i = 0; i < m; i++) {
        int u, v, length;
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

    int* stateSizes = calloc(k, sizeof(int));
    for (size_t i = 0; i < partition.size; i++) {
        stateSizes[partition.data[i].state]++;
    }

    int** groups = calloc(k, sizeof(int*));
    for (int i = 0; i < k; i++) {
        if (stateSizes[i] > 0) {
            groups[i] = malloc(stateSizes[i] * sizeof(int));
        }
    }

    int* offsets = calloc(k, sizeof(int));
    for (size_t i = 0; i < partition.size; i++) {
        int state = partition.data[i].state;
        int city = partition.data[i].city;

        groups[state][offsets[state]++] = city;
    }

    for (int state = 0; state < k; state++) {
        printf("%d:", state);
        for (int i = 0; i < stateSizes[state]; i++) {
            printf(" %d", groups[state][i]);
        }

        printf("\n");
    }

    for (int i = 0; i < k; i++) {
        free(groups[i]);
    }

    free(groups);
    free(stateSizes);
    free(offsets);

    partitionDelete(&partition);
    graphDelete(&graph);
    free(capitals);

    return 0;
}
