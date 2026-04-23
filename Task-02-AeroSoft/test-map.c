#include "map.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

// Helper functions for string values (char*)
static MapValue stringCopy(MapValue value)
{
    if (!value)
        return NULL;
    return strdup((char*)value);
}

static void stringFree(MapValue value)
{
    free(value);
}

void testCreateEmptyMap()
{
    Map* map = NULL;
    assert(mapCreate(&map, stringCopy, stringFree) == MapSucsess);
    assert(map);

    size_t size = 0;
    assert(mapGetSize(map, &size) == MapSucsess);
    assert(size == 0);

    assert(mapContains(map, 42) == MapErrKeyNotFound);

    assert(mapDelete(&map) == MapSucsess);
    assert(!map);
}

void testSetAndGet()
{
    Map* map = NULL;
    assert(mapCreate(&map, stringCopy, stringFree) == MapSucsess);

    char* original = "hello";
    assert(mapSet(map, 1, original) == MapSucsess);

    MapValue retrieved = NULL;
    assert(mapGet(map, 1, &retrieved) == MapSucsess);
    assert(retrieved);
    assert(strcmp((char*)retrieved, "hello") == 0);
    assert(retrieved != original);

    assert(mapDelete(&map) == MapSucsess);
}

void testContainsAndErase()
{
    Map* map = NULL;
    assert(mapCreate(&map, stringCopy, stringFree) == MapSucsess);

    assert(mapSet(map, 10, "ten") == MapSucsess);
    assert(mapContains(map, 10) == MapSucsess);
    assert(mapContains(map, 20) == MapErrKeyNotFound);

    assert(mapErase(map, 10) == MapSucsess);
    assert(mapContains(map, 10) == MapErrKeyNotFound);
    assert(mapErase(map, 999) == MapErrKeyNotFound);

    assert(mapDelete(&map) == MapSucsess);
}

void testIterator()
{
    Map* map = NULL;
    assert(mapCreate(&map, stringCopy, stringFree) == MapSucsess);

    mapSet(map, 1, "one");
    mapSet(map, 2, "two");
    mapSet(map, 3, "three");

    Iterator* it = iteratorInit(map);
    assert(it);

    int count = 0;
    while (iteratorHasNext(it)) {
        MapEntry entry = iteratorNext(it);
        assert(entry.key >= 1 && entry.key <= 3);
        assert(entry.value != NULL);
        count++;
    }
    assert(count == 3);

    iteratorDelete(it);
    assert(mapDelete(&map) == MapSucsess);
}

void testOverwriteKey()
{
    Map* map = NULL;
    assert(mapCreate(&map, stringCopy, stringFree) == MapSucsess);

    assert(mapSet(map, 5, "first") == MapSucsess);
    assert(mapSet(map, 5, "second") == MapSucsess);

    MapValue val = NULL;
    assert(mapGet(map, 5, &val) == MapSucsess);
    assert(strcmp((char*)val, "second") == 0);

    size_t size = 0;
    assert(mapGetSize(map, &size) == MapSucsess);
    assert(size == 1);

    assert(mapDelete(&map) == MapSucsess);
}

int main(void)
{
    testCreateEmptyMap();
    testSetAndGet();
    testContainsAndErase();
    testIterator();
    testOverwriteKey();

    return 0;
}
