#include "map.h"
#include <stdlib.h>

typedef struct MapNode {
    MapKey key;
    MapValue value;
    int height;
    struct MapNode* left;
    struct MapNode* right;
} MapNode;

typedef struct Map {
    MapNode* root;
    size_t size;
    MapValueCopyFunc copyFunc;
    MapValueFreeFunc freeFunc;
} Map;


MapReturnCode mapCreate(Map** map, MapValueCopyFunc copyFunc, MapValueFreeFunc freeFunc)
{
    if (map == NULL) {
        return MapErrNoMap;
    }

    *map = malloc(sizeof(Map));
    if (*map == NULL) {
        return MapErrOnMalloc;
    }

    (*map)->root = NULL;
    (*map)->size = 0;
    (*map)->copyFunc = copyFunc;
    (*map)->freeFunc = freeFunc;

    return MapSucsess;
}


MapReturnCode mapSet(Map* map, MapKey key, MapValue value)
{
    if (map == NULL) {
        return MapErrNoMap;
    }

    return MapSucsess;
}


MapReturnCode mapGet(Map* map, MapKey key, MapValue* value)
{
    if (map == NULL) {
        return MapErrNoMap;
    }

    return MapSucsess;
}


MapReturnCode mapContains(Map* map, MapKey key)
{
    if (map == NULL) {
        return MapErrNoMap;
    }

    return MapSucsess;
}


MapReturnCode mapErase(Map* map, MapKey key)
{
    if (map == NULL) {
        return MapErrNoMap;
    }

    return MapSucsess;
}


MapReturnCode mapGetSize(Map* map, size_t* mapSize)
{
    if (map == NULL) {
        return MapErrNoMap;
    }

    return MapSucsess;
}


MapReturnCode mapDelete(Map** map)
{
    if (map == NULL) {
        return MapErrNoMap;
    }

    return MapSucsess;
}
