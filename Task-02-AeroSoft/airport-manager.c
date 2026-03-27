#include "airport-manager.h"

#include "map.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

MapValue copyString(MapValue value)
{
    if (value == NULL) {
        return NULL;
    }

    return strdup((char*)value);
}

void freeString(MapValue value)
{
    if (value == NULL) {
        return;
    }

    free(value);
}

bool isIataCorrect(const char* iataCode)
{
    if (iataCode == NULL || strlen(iataCode) != 3) {
        return false;
    }

    return (isupper(iataCode[0]) && isupper(iataCode[1]) && isupper(iataCode[2]));
}

MapKey iataToKey(const char* code)
{
    return (code[0] << 16) | (code[1] << 8) | code[2];
}


typedef struct AirportManager {
    Map* data;
} AirportManager;


AirportManagerReturnCode amCreateManager(AirportManager** manager)
{
    if (manager == NULL) {
        return AmErrNoManager;
    }

    *manager = malloc(sizeof(AirportManager));
    if ((*manager) == NULL) {
        return AmErrOnMalloc;
    }

    MapReturnCode mapCode = mapCreate(&((*manager)->data), copyString, freeString);
    if (mapCode == MapErrOnMalloc) {
        free(*manager);
        *manager = NULL;
        return AmErrOnMalloc;
    }

    return AmSucsess;
}

AirportManagerReturnCode amInitFromFile(AirportManager* manager, const char* path, size_t* airportsNumber)
{
    return AmSucsess;
}

AirportManagerReturnCode amFindAirport(AirportManager* manager, const char* iataCode, char* nameBuffer, size_t bufferSize)
{
    return AmSucsess;
}

AirportManagerReturnCode amAddAirport(AirportManager* manager, const char* iataCode, const char* name)
{
    return AmSucsess;
}

AirportManagerReturnCode amDeleteAirport(AirportManager* manager, const char* iataCode)
{
    return AmSucsess;
}

AirportManagerReturnCode amSaveToFile(AirportManager* manager, const char* path, size_t* airportsNumber)
{
    return AmSucsess;
}

AirportManagerReturnCode amDeleteManager(AirportManager** manager)
{
    return AmSucsess;
}
