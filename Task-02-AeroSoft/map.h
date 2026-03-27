#pragma once

#include <stddef.h>
#include <stdbool.h>

typedef int MapKey;
typedef void* MapValue;

typedef MapValue (*MapValueCopyFunc)(MapValue value);
typedef void (*MapValueFreeFunc)(MapValue value);

typedef struct Map Map;

typedef enum {
    MapSucsess,
    MapErrNoMap,
    MapErrOnMalloc,
    MapErrKeyNotFound
} MapReturnCode;

// MapSucsess, MapErrOnMalloc
MapReturnCode mapCreate(Map** map, MapValueCopyFunc copyFunc, MapValueFreeFunc freeFunc);

// MapSucsess, MapErrNoMap, MapErrOnMalloc
MapReturnCode mapSet(Map* map, MapKey key, MapValue value);

// MapSucsess, MapErrNoMap, MapErrKeyNotFound
MapReturnCode mapGet(Map* map, MapKey key, MapValue* value);

// MapSucsess, MapErrNoMap, MapErrKeyNotFound
MapReturnCode mapContains(Map* map, MapKey key);

// MapSucsess, MapErrNoMap, MapErrKeyNotFound
MapReturnCode mapErase(Map* map, MapKey key);

// MapSucsess, MapErrNoMap
MapReturnCode mapGetSize(Map* map, size_t* mapSize);

// MapSucsess, MapErrNoMap
MapReturnCode mapDelete(Map** map);

typedef struct Iterator Iterator;
typedef struct MapEntry {
    MapKey key;
    MapValue value;
} MapEntry;

Iterator* iteratorInit(Map* map);

bool iteratorHasNext(Iterator* it);

MapEntry iteratorNext(Iterator* it);

void iteratorDelete(Iterator* it);
