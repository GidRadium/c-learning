#include "airport-manager.h"

#include "map.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>

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

MapKey iataToKey(const char* iataCode)
{
    MapKey key = 0;
    memcpy(&key, iataCode, 3);
    return key;
}

void keyToIata(MapKey key, char code[4])
{
    memcpy(code, &key, 3);
    code[3] = '\0';
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
    if (manager == NULL) {
        return AmErrNoManager;
    }

    if (path == NULL) {
        return AmErrNoFile;
    }

    FILE* file = fopen(path, "r");
    if (file == NULL) {
        return AmErrNoFile;
    }

    *airportsNumber = 0;
    char lineBuffer[1024];
    char codeBuffer[4];
    char nameBuffer[1020];
    while (fgets(lineBuffer, sizeof(lineBuffer), file)) {
        if (sscanf(lineBuffer, "%3[^:]:%255[^\n]", codeBuffer, nameBuffer) != 2) {
            continue;
        }

        if (!isIataCorrect(codeBuffer)) {
            continue;
        }

        if (amAddAirport(manager, codeBuffer, nameBuffer) == AmSucsess) {
            (*airportsNumber)++;
        }
    }

    return AmSucsess;
}

AirportManagerReturnCode amFindAirport(AirportManager* manager, const char* iataCode, char* nameBuffer, size_t bufferSize)
{
    if (manager == NULL) {
        return AmErrNoManager;
    }

    if (!isIataCorrect(iataCode)) {
        return AmErrIataCodeIncorrect;
    }

    MapValue value;
    if (mapGet(manager->data, iataToKey(iataCode), &value) != MapSucsess) {
        return AmErrAirportNotFound;
    }

    if (strlen((char*)value) >= bufferSize) {
        return AmErrNameTooLong;
    }

    strcpy(nameBuffer, (char*)value);

    return AmSucsess;
}

AirportManagerReturnCode amAddAirport(AirportManager* manager, const char* iataCode, const char* name)
{
    if (manager == NULL) {
        return AmErrNoManager;
    }

    if (!isIataCorrect(iataCode)) {
        return AmErrIataCodeIncorrect;
    }

    if (mapContains(manager->data, iataToKey(iataCode)) == MapSucsess) {
        return AmErrAirportAlreadyExist;
    }

    if (mapSet(manager->data, iataToKey(iataCode), (MapValue)name) != MapSucsess) {
        return AmErrOnMalloc;
    }

    return AmSucsess;
}

AirportManagerReturnCode amDeleteAirport(AirportManager* manager, const char* iataCode)
{
    if (manager == NULL) {
        return AmErrNoManager;
    }

    if (!isIataCorrect(iataCode)) {
        return AmErrIataCodeIncorrect;
    }

    if (mapErase(manager->data, iataToKey(iataCode)) != MapSucsess) {
        return AmErrAirportNotFound;
    }

    return AmSucsess;
}

AirportManagerReturnCode amSaveToFile(AirportManager* manager, const char* path, size_t* airportsNumber)
{
    if (manager == NULL) {
        return AmErrNoManager;
    }

    FILE* file = fopen(path, "w");
    if (file == NULL) {
        return AmErrNoFile;
    }

    Iterator* it = iteratorInit(manager->data);
    if (it == NULL) {
        fclose(file);
        return AmErrOnMalloc;
    }

    *airportsNumber = 0;
    while (iteratorHasNext(it)) {
        MapEntry entry = iteratorNext(it);

        char iataCode[4];
        keyToIata(entry.key, iataCode);

        fprintf(file, "%s:%s\n", iataCode, (char*)entry.value);

        (*airportsNumber)++;
    }

    iteratorDelete(it);
    fclose(file);

    return AmSucsess;
}

AirportManagerReturnCode amDeleteManager(AirportManager** manager)
{
    if (manager == NULL || (*manager) == NULL) {
        return AmErrNoManager;
    }

    mapDelete(&((*manager)->data));
    free(*manager);
    *manager = NULL;

    return AmSucsess;
}
