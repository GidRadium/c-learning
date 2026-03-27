#pragma once

#include <stddef.h>
#include <stdbool.h>

// Data key for MapValue stored in Map struct.
typedef int MapKey;

// Data unit stored in Map struct.
typedef void* MapValue;

// Deep copy function for value.
typedef MapValue (*MapValueCopyFunc)(MapValue value);

// Deep free function for value.
typedef void (*MapValueFreeFunc)(MapValue value);

// Struct that maps MapKey to MapValue. Contains all deep copies of values.
typedef struct Map Map;

// Possible return codes of Map struct functions.
typedef enum {
    MapSucsess,
    MapErrNoMap,
    MapErrOnMalloc,
    MapErrKeyNotFound
} MapReturnCode;

// Allocates memmory for Map. Saves copy and free funcs of value.
// Possible return codes: MapSucsess, MapErrOnMalloc
MapReturnCode mapCreate(Map** map, MapValueCopyFunc copyFunc, MapValueFreeFunc freeFunc);

// Sets value by key. Takes deep copy of value, deletes old value by same key if required.
// Possible return codes: MapSucsess, MapErrNoMap, MapErrOnMalloc
MapReturnCode mapSet(Map* map, MapKey key, MapValue value);

// Sets value as pointer to value-by-key from map. Does not copy it.
// Possible return codes: MapSucsess, MapErrNoMap, MapErrKeyNotFound
MapReturnCode mapGet(Map* map, MapKey key, MapValue* value);

// Checks for key. Returns MapSucsess if it contains.
// Possible return codes: MapSucsess, MapErrNoMap, MapErrKeyNotFound
MapReturnCode mapContains(Map* map, MapKey key);

// Erases element by key. Uses MapValueFreeFunc on it.
// Possible return codes: MapSucsess, MapErrNoMap, MapErrKeyNotFound
MapReturnCode mapErase(Map* map, MapKey key);

// Sets mapSize to number of elements stored in map.
// Possible return codes: MapSucsess, MapErrNoMap
MapReturnCode mapGetSize(Map* map, size_t* mapSize);

// Frees (MapValueFreeFunc) all stored elements. Frees Map struct. Sets it to NULL.
// Possible return codes: MapSucsess, MapErrNoMap
MapReturnCode mapDelete(Map** map);

// Iterator strict for Map struct.
typedef struct Iterator Iterator;

// Key and value pair struct.
typedef struct MapEntry {
    MapKey key;
    MapValue value;
} MapEntry;

// Allocates memory for Iterator, checks every entry in map.
Iterator* iteratorInit(Map* map);

// Returs true if it is not last iteration. Does not change map.
bool iteratorHasNext(Iterator* it);

// Returns key and value. Goes to next entry.
MapEntry iteratorNext(Iterator* it);

// Free all memory allocated for Iterator structure. Does not free values data.
void iteratorDelete(Iterator* it);
