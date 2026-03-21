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

typedef struct AirportManager AirportManager;

// AmSucsess, AmErrOnMalloc
AirportManagerReturnCode amCreateManager(AirportManager** manager);

// AmSucsess, AmErrNoManager, AmErrNoFile, AmErrOnMalloc
AirportManagerReturnCode amInitFromFile(AirportManager* manager, const char* path, size_t* airportsNumber);

// AmSucsess, AmErrNoManager, AmErrIataCodeIncorrect, AmErrAirportNotFound, AmErrNameTooLong
AirportManagerReturnCode amFindAirport(AirportManager* manager, const char* iataCode, char* nameBuffer, size_t bufferSize);

// AmSucsess, AmErrNoManager, AmErrIataCodeIncorrect, AmErrAirportAlreadyExist, AmErrOnMalloc
AirportManagerReturnCode amAddAirport(AirportManager* manager, const char* iataCode, const char* name);

// AmSucsess, AmErrNoManager, AmErrIataCodeIncorrect, AmErrAirportNotFound
AirportManagerReturnCode amDeleteAirport(AirportManager* manager, const char* iataCode);

// AmSucsess, AmErrNoManager, AmErrNoFile
AirportManagerReturnCode amSaveToFile(AirportManager* manager, const char* path, size_t* airportsNumber);

// AmSucsess, AmErrNoManager
AirportManagerReturnCode amDeleteManager(AirportManager **manager);
