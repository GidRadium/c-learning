#include "graph.h"
#include <limits.h>
#include <stdlib.h>

typedef struct Edge {
    int destination;
    int length;
    struct Edge* next;
} Edge;

struct Graph {
    int numberOfVertices;
    int numberOfEdges;
    Edge** adjacencyList;
};

typedef struct {
    int city;
    int state;
    int distance;
} pqNode;

typedef struct {
    pqNode* elements;
    int currentSize;
    int capacity;
} PriorityQueue;

static void pqSwap(pqNode* left, pqNode* right)
{
    pqNode temp = *left;
    *left = *right;
    *right = temp;
}

static void pqPush(PriorityQueue* queue, pqNode node)
{
    int index = queue->currentSize++;
    queue->elements[index] = node;

    while (index > 0) {
        int parentIndex = (index - 1) / 2;
        if (queue->elements[parentIndex].distance <= queue->elements[index].distance) {
            break;
        }

        pqSwap(&queue->elements[parentIndex], &queue->elements[index]);
        index = parentIndex;
    }
}

static pqNode pqPop(PriorityQueue* queue)
{
    pqNode resultNode = queue->elements[0];
    queue->elements[0] = queue->elements[--queue->currentSize];

    int i = 0;
    while (1) {
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        int minI = i;

        if (l < queue->currentSize && queue->elements[l].distance < queue->elements[minI].distance) {
            minI = l;
        }

        if (r < queue->currentSize && queue->elements[r].distance < queue->elements[minI].distance) {
            minI = r;
        }

        if (minI == i)
            break;

        pqSwap(&queue->elements[i], &queue->elements[minI]);
        i = minI;
    }

    return resultNode;
}

Graph* graphCreate(int numberOfVertices, int numberOfEdges)
{
    Graph* graph = malloc(sizeof(Graph));

    graph->numberOfVertices = numberOfVertices;
    graph->numberOfEdges = numberOfEdges;
    graph->adjacencyList = calloc(numberOfVertices + 1, sizeof(Edge*));

    return graph;
}

void graphAddEdge(Graph* graph, int u, int v, int length)
{
    Edge* e1 = malloc(sizeof(Edge));
    e1->destination = v;
    e1->length = length;
    e1->next = graph->adjacencyList[u];
    graph->adjacencyList[u] = e1;

    Edge* e2 = malloc(sizeof(Edge));
    e2->destination = u;
    e2->length = length;
    e2->next = graph->adjacencyList[v];
    graph->adjacencyList[v] = e2;
}

Partition graphSolve(Graph* graph, int numberOfCapitals, const int* capitals)
{
    int numberOfCities = graph->numberOfVertices;

    int* cityState = malloc((numberOfCities + 1) * sizeof(int));

    for (int i = 1; i <= numberOfCities; i++)
        cityState[i] = -1;

    PriorityQueue pq;
    pq.capacity = graph->numberOfEdges * 2 + 10;
    pq.currentSize = 0;
    pq.elements = malloc(sizeof(pqNode) * pq.capacity);

    for (int i = 0; i < numberOfCapitals; i++) {
        int capital = capitals[i];

        cityState[capital] = i;

        for (Edge* e = graph->adjacencyList[capital]; e != NULL; e = e->next) {
            pqPush(&pq, (pqNode) { e->destination, i, e->length });
        }
    }

    while (pq.currentSize > 0) {
        pqNode currentNode = pqPop(&pq);

        if (cityState[currentNode.city] != -1) {
            continue;
        }

        cityState[currentNode.city] = currentNode.state;
        for (Edge* e = graph->adjacencyList[currentNode.city]; e != NULL; e = e->next) {
            if (cityState[e->destination] == -1) {
                pqPush(&pq, (pqNode) { e->destination, currentNode.state, e->length });
            }
        }
    }

    Partition partition;
    partition.size = numberOfCities;
    partition.data = malloc(numberOfCities * sizeof(PartitionEntry));

    for (int i = 1; i <= numberOfCities; i++) {
        partition.data[i - 1].city = i;
        partition.data[i - 1].state = cityState[i];
    }

    free(cityState);
    free(pq.elements);

    return partition;
}

void graphDestroy(Graph** graph)
{
    if (graph == NULL || *graph == NULL) {
        return;
    }

    for (int i = 1; i <= (*graph)->numberOfVertices; i++) {
        Edge* e = (*graph)->adjacencyList[i];

        while (e != NULL) {
            Edge* next = e->next;
            free(e);
            e = next;
        }
    }

    free((*graph)->adjacencyList);
    free(*graph);

    *graph = NULL;
}

void partitionDelete(Partition* partition)
{
    if (!partition) {
        return;
    }

    free(partition->data);
}
