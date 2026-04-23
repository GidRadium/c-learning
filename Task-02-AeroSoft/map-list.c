#include "map.h"
#include <stdlib.h>

typedef struct MapNode {
    MapKey key;
    MapValue value;
    struct MapNode* next;
} MapNode;

typedef struct Map {
    MapNode* head;
    size_t size;
    MapValueCopyFunc copyFunc;
    MapValueFreeFunc freeFunc;
} Map;

static MapNode** nodeFindRef(MapNode** nodeRef, MapKey key)
{
    while (*nodeRef != NULL) {
        if (key == (*nodeRef)->key) {
            return nodeRef;
        }
        nodeRef = &(*nodeRef)->next;
    }
    return NULL;
}

static MapNode* nodeCreate(MapKey key, MapValue valueDeepCopy)
{
    MapNode* node = malloc(sizeof(MapNode));
    if (node == NULL) {
        return NULL;
    }

    node->key = key;
    node->value = valueDeepCopy;
    node->next = NULL;

    return node;
}

MapReturnCode mapCreate(Map** map, MapValueCopyFunc copyFunc, MapValueFreeFunc freeFunc)
{
    if (map == NULL) {
        return MapErrNoMap;
    }

    *map = malloc(sizeof(Map));
    if (*map == NULL) {
        return MapErrOnMalloc;
    }

    (*map)->head = NULL;
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

    MapNode** existingRef = nodeFindRef(&map->head, key);

    MapValue newValue = map->copyFunc(value);
    if (newValue == NULL && value != NULL) {
        return MapErrOnMalloc;
    }

    if (existingRef != NULL) {
        map->freeFunc((*existingRef)->value);
        (*existingRef)->value = newValue;
        return MapSucsess;
    }

    MapNode* newNode = nodeCreate(key, newValue);
    if (newNode == NULL) {
        map->freeFunc(newValue);
        return MapErrOnMalloc;
    }

    newNode->next = map->head;
    map->head = newNode;
    map->size++;

    return MapSucsess;
}

MapReturnCode mapGet(Map* map, MapKey key, MapValue* value)
{
    if (map == NULL) {
        return MapErrNoMap;
    }

    MapNode** nodeRef = nodeFindRef(&map->head, key);
    if (nodeRef == NULL) {
        return MapErrKeyNotFound;
    }

    if (value != NULL) {
        *value = (*nodeRef)->value;
    }

    return MapSucsess;
}

MapReturnCode mapContains(Map* map, MapKey key)
{
    if (map == NULL) {
        return MapErrNoMap;
    }

    if (nodeFindRef(&map->head, key) == NULL) {
        return MapErrKeyNotFound;
    }

    return MapSucsess;
}

MapReturnCode mapErase(Map* map, MapKey key)
{
    if (map == NULL) {
        return MapErrNoMap;
    }

    MapNode** nodeRef = nodeFindRef(&map->head, key);
    if (nodeRef == NULL) {
        return MapErrKeyNotFound;
    }

    MapNode* nodeToDelete = *nodeRef;
    *nodeRef = nodeToDelete->next;

    map->freeFunc(nodeToDelete->value);
    free(nodeToDelete);
    map->size--;

    return MapSucsess;
}

MapReturnCode mapGetSize(Map* map, size_t* mapSize)
{
    if (map == NULL) {
        return MapErrNoMap;
    }

    if (mapSize != NULL) {
        *mapSize = map->size;
    }

    return MapSucsess;
}

MapReturnCode mapDelete(Map** map)
{
    if (map == NULL || (*map) == NULL) {
        return MapErrNoMap;
    }

    MapNode* current = (*map)->head;
    while (current != NULL) {
        MapNode* next = current->next;
        (*map)->freeFunc(current->value);
        free(current);
        current = next;
    }

    free(*map);
    *map = NULL;

    return MapSucsess;
}

struct Iterator {
    MapNode* current;
};

Iterator* iteratorInit(Map* map)
{
    if (map == NULL) {
        return NULL;
    }

    Iterator* it = malloc(sizeof(Iterator));
    if (it == NULL) {
        return NULL;
    }

    it->current = map->head;
    return it;
}

bool iteratorHasNext(Iterator* it)
{
    if (it == NULL) {
        return false;
    }

    return it->current != NULL;
}

MapEntry iteratorNext(Iterator* it)
{
    MapEntry entry = { 0 };

    if (it == NULL || it->current == NULL) {
        return entry;
    }

    entry.key = it->current->key;
    entry.value = it->current->value;
    it->current = it->current->next;

    return entry;
}

void iteratorDelete(Iterator* it)
{
    if (it != NULL) {
        free(it);
    }
}
