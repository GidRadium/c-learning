#include "graph.h"
#include <assert.h>

void testSingleCitySingleCapital()
{
    Graph* g = graphCreate(1, 0);

    int capitals[] = { 1 };
    Partition p = graphSolve(g, 1, capitals);

    assert(p.size == 1);
    assert(p.data[0].city == 1);
    assert(p.data[0].state == 0);

    partitionDelete(&p);
    graphDelete(&g);
}

void testLinearGraphTwoCapitals()
{
    Graph* g = graphCreate(4, 3);

    graphAddEdge(g, 1, 2, 10);
    graphAddEdge(g, 2, 3, 10);
    graphAddEdge(g, 3, 4, 10);

    int capitals[] = { 1, 4 };
    Partition p = graphSolve(g, 2, capitals);

    assert(p.size == 4);

    assert(p.data[0].city == 1 && p.data[0].state == 0);
    assert(p.data[1].city == 2 && p.data[1].state == 0);
    assert(p.data[2].city == 3 && p.data[2].state == 1);
    assert(p.data[3].city == 4 && p.data[3].state == 1);

    partitionDelete(&p);
    graphDelete(&g);
}

void testStarGraphCloserCapital()
{
    Graph* g = graphCreate(4, 3);
    graphAddEdge(g, 1, 2, 5);
    graphAddEdge(g, 1, 3, 1);
    graphAddEdge(g, 1, 4, 1);

    int capitals[] = { 2, 3 };
    Partition p = graphSolve(g, 2, capitals);

    assert(p.size == 4);

    assert(p.data[0].city == 1 && p.data[0].state == 1);
    assert(p.data[1].city == 2 && p.data[1].state == 0);
    assert(p.data[2].city == 3 && p.data[2].state == 1);
    assert(p.data[3].city == 4 && p.data[3].state == 1);

    partitionDelete(&p);
    graphDelete(&g);
}

int main(void)
{
    testSingleCitySingleCapital();
    testLinearGraphTwoCapitals();
    testStarGraphCloserCapital();

    return 0;
}
