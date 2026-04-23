#include "airport-manager.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST_FILE "test_airports.txt"
#define BUFFER_SIZE 256

void testCreateManager()
{
    AirportManager* manager = NULL;
    assert(amCreateManager(&manager) == AmSucsess);
    assert(manager);

    assert(amDeleteManager(&manager) == AmSucsess);
    assert(!manager);
}

void testAddAndFind()
{
    AirportManager* manager = NULL;
    assert(amCreateManager(&manager) == AmSucsess);

    assert(amAddAirport(manager, "SVO", "Sheremetyevo International Airport") == AmSucsess);

    char buffer[BUFFER_SIZE] = { 0 };
    assert(amFindAirport(manager, "SVO", buffer, BUFFER_SIZE) == AmSucsess);
    assert(strcmp(buffer, "Sheremetyevo International Airport") == 0);

    memset(buffer, 0, BUFFER_SIZE);
    assert(amFindAirport(manager, "ZZZ", buffer, BUFFER_SIZE) == AmErrAirportNotFound);
    assert(buffer[0] == '\0');

    assert(amDeleteManager(&manager) == AmSucsess);
}

void testDeleteAirport()
{
    AirportManager* manager = NULL;
    assert(amCreateManager(&manager) == AmSucsess);

    assert(amAddAirport(manager, "DXB", "Dubai International") == AmSucsess);
    assert(amFindAirport(manager, "DXB", NULL, 0) == AmErrNameTooLong);

    assert(amDeleteAirport(manager, "DXB") == AmSucsess);
    assert(amFindAirport(manager, "DXB", NULL, 0) == AmErrAirportNotFound);

    assert(amDeleteAirport(manager, "XXX") == AmErrAirportNotFound);

    assert(amDeleteManager(&manager) == AmSucsess);
}

void testInvalidIataCode()
{
    AirportManager* manager = NULL;
    assert(amCreateManager(&manager) == AmSucsess);

    assert(amAddAirport(manager, "AB", "Too Short") == AmErrIataCodeIncorrect);
    assert(amAddAirport(manager, "ABCD", "Too Long") == AmErrIataCodeIncorrect);
    assert(amAddAirport(manager, "ab1", "Lowercase/Digit") == AmErrIataCodeIncorrect);
    assert(amAddAirport(manager, "A-B", "With Special Char") == AmErrIataCodeIncorrect);

    assert(amDeleteManager(&manager) == AmSucsess);
}

void testDuplicateAdd()
{
    AirportManager* manager = NULL;
    assert(amCreateManager(&manager) == AmSucsess);

    assert(amAddAirport(manager, "LHR", "Heathrow") == AmSucsess);
    assert(amAddAirport(manager, "LHR", "Heathrow Updated") == AmErrAirportAlreadyExist);

    char buffer[BUFFER_SIZE] = { 0 };
    assert(amFindAirport(manager, "LHR", buffer, BUFFER_SIZE) == AmSucsess);
    assert(strcmp(buffer, "Heathrow") == 0);

    assert(amDeleteManager(&manager) == AmSucsess);
}

void testSaveAndLoad()
{
    AirportManager* manager = NULL;
    assert(amCreateManager(&manager) == AmSucsess);

    amAddAirport(manager, "JFK", "John F Kennedy International");
    amAddAirport(manager, "LAX", "Los Angeles International");

    size_t savedCount = 0;
    assert(amSaveToFile(manager, TEST_FILE, &savedCount) == AmSucsess);
    assert(savedCount == 2);

    AirportManager* loaded = NULL;
    assert(amCreateManager(&loaded) == AmSucsess);

    size_t loadedCount = 0;
    assert(amInitFromFile(loaded, TEST_FILE, &loadedCount) == AmSucsess);
    assert(loadedCount == 2);

    char buffer[BUFFER_SIZE] = { 0 };
    assert(amFindAirport(loaded, "JFK", buffer, BUFFER_SIZE) == AmSucsess);
    assert(strcmp(buffer, "John F Kennedy International") == 0);

    assert(amDeleteManager(&loaded) == AmSucsess);
    assert(amDeleteManager(&manager) == AmSucsess);

    remove(TEST_FILE);
}

int main(void)
{
    testCreateManager();
    testAddAndFind();
    testDeleteAirport();
    testInvalidIataCode();
    testDuplicateAdd();
    testSaveAndLoad();

    return 0;
}
