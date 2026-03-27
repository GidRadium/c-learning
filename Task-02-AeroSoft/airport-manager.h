#pragma once

#include <stddef.h>

typedef enum {
    AmSucsess,
    AmErrNoManager,
    AmErrNoFile,
    AmErrOnMalloc,
    AmErrIataCodeIncorrect,
    AmErrAirportAlreadyExist,
    AmErrAirportNotFound,
    AmErrNameTooLong,

} AirportManagerReturnCode;

// Struct that manages airports by their IATA codes.
typedef struct AirportManager AirportManager;

// Allocates memmory for manager struct. Sets manager as NULL if something went wrong.
// Possible return codes: AmSucsess, AmErrNoManager, AmErrOnMalloc
AirportManagerReturnCode amCreateManager(AirportManager** manager);

// Opens file and reads table in IATA:NAME format. Closes file. Allocates memmory for all data.
// Possible return codes: AmSucsess, AmErrNoManager, AmErrNoFile, AmErrOnMalloc
AirportManagerReturnCode amInitFromFile(AirportManager* manager, const char* path, size_t* airportsNumber);

// Copy name of airport to buffer by it IATA code. Otherwise do nothing with buffer.
// Possible return codes: AmSucsess, AmErrNoManager, AmErrIataCodeIncorrect, AmErrAirportNotFound, AmErrNameTooLong
AirportManagerReturnCode amFindAirport(AirportManager* manager, const char* iataCode, char* nameBuffer, size_t bufferSize);

// Adds airport name by IATA code. Do nothing if already exists. Allocates memmory for it.
// Possible return codes: AmSucsess, AmErrNoManager, AmErrIataCodeIncorrect, AmErrAirportAlreadyExist, AmErrOnMalloc
AirportManagerReturnCode amAddAirport(AirportManager* manager, const char* iataCode, const char* name);

// Deletes airport by it IATA code. Do nothing if not exists.
// Possible return codes: AmSucsess, AmErrNoManager, AmErrIataCodeIncorrect, AmErrAirportNotFound
AirportManagerReturnCode amDeleteAirport(AirportManager* manager, const char* iataCode);

// Saves all data to the file in IATA:NAME format. Rewrites it if already exists.
// Possible return codes: AmSucsess, AmErrNoManager, AmErrNoFile
AirportManagerReturnCode amSaveToFile(AirportManager* manager, const char* path, size_t* airportsNumber);

// Deletes manager struct. Free all allocated data. Sets it to NULL.
// Possible return codes: AmSucsess, AmErrNoManager
AirportManagerReturnCode amDeleteManager(AirportManager** manager);
