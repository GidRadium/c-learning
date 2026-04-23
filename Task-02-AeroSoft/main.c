#include "airport-manager.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* getArgument(char* string, char stopSymbol, char** otherArguments)
{
    if (string == NULL) {
        if (otherArguments != NULL) {
            (*otherArguments) = string;
        }

        return NULL;
    }

    size_t length = strlen(string);
    size_t start = 0;
    while (start < length && string[start] == ' ') {
        start++;
    }

    size_t end = start;
    while (end < length && string[end] != stopSymbol && string[end] != '\n' && string[end] != '\r') {
        end++;
    }

    if (end == start) {
        if (otherArguments != NULL) {
            (*otherArguments) = string;
        }

        return NULL;
    }

    if (end < length) {
        string[end] = '\0';
    }

    if (otherArguments != NULL) {
        if (end + 1 < length) {
            (*otherArguments) = string + (end + 1);
        } else {
            (*otherArguments) = NULL;
        }
    }
    return string + start;
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Ошибка! Неверное количество аргументов.\n");
        return -1;
    }

    char* errUnknown = "Ошибка! Неожиданная ошибка!";
    char* errOnMalloc = "Ошибка! Не удалось выделить память.";
    char* errNoFile = "Ошибка! Не удалось прочитать файл.";
    char* errIataCodeIncorrect = "Ошибка! Неверный формат кода IATA.";
    char* errAirportAlreadyExist = "Ошибка! Аэропорт уже существует в базе.";
    char* errAirportNotFound = "Ошибка! Аэропорт не найден в базе.";
    char* errNameTooLong = "Ошибка! Название аэропорта слишком длинное.";
    char* errNoManager = "Ошибка! Менеджер не инициализирован.";
    char* errNoCode = "Ошибка! Не указан код IATA.";
    char* errNoName = "Ошибка! Не указано название аэропорта.";
    char* errAddFormat = "Ошибка! Формат: add <код>:<название>";
    char* errUnknownCommand = "Ошибка! Неизвестная команда.";
    char* errReadQuery = "Ошибка! Не удалось прочитать запрос.";

    char* path = argv[1];

    AirportManagerReturnCode rc;
    AirportManager* manager = NULL;

    rc = amCreateManager(&manager);
    switch (rc) {
    case AmSucsess:
        break;
    case AmErrOnMalloc:
        printf("%s\n", errOnMalloc);
        return -1;
    case AmErrNoManager:
        printf("%s\n", errNoManager);
        return -1;
    default:
        printf("%s\n", errUnknown);
        return -1;
    }

    size_t counter = 0;
    rc = amInitFromFile(manager, path, &counter);
    switch (rc) {
    case AmSucsess:
        printf("Загружено %zu аэропортов. Система готова к работе.\n", counter);
        break;
    case AmErrNoFile:
        printf("%s (%s)\n", errNoFile, path);
        amDeleteManager(&manager);
        return -1;
    case AmErrOnMalloc:
        printf("%s\n", errOnMalloc);
        amDeleteManager(&manager);
        return -1;
    case AmErrNoManager:
        printf("%s\n", errNoManager);
        amDeleteManager(&manager);
        return -1;
    default:
        printf("%s\n", errUnknown);
        amDeleteManager(&manager);
        return -1;
    }

    while (true) {
        char buffer[512];

        printf("> ");
        fflush(stdout);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("%s\n", errReadQuery);
            break;
        }

        char* arguments = NULL;
        char* command = getArgument(buffer, ' ', &arguments);
        if (command == NULL) {
            printf("%s\n", errUnknownCommand);
            continue;
        }

        if (strcmp(command, "quit") == 0) {
            break;
        } else if (strcmp(command, "find") == 0) {
            char* iataCode = getArgument(arguments, '\n', NULL);
            if (iataCode == NULL || iataCode[0] == '\0') {
                printf("%s\n", errNoCode);
                continue;
            }

            char nameBuffer[1024];
            rc = amFindAirport(manager, iataCode, nameBuffer, sizeof(nameBuffer));
            switch (rc) {
            case AmSucsess:
                printf("%s → %s\n", iataCode, nameBuffer);
                break;
            case AmErrIataCodeIncorrect:
                printf("%s\n", errIataCodeIncorrect);
                break;
            case AmErrAirportNotFound:
                printf("Аэропорт с кодом '%s' не найден в базе.\n", iataCode);
                break;
            case AmErrNameTooLong:
                printf("%s\n", errNameTooLong);
                break;
            case AmErrNoManager:
                printf("%s\n", errNoManager);
                break;
            default:
                printf("%s\n", errUnknown);
                break;
            }
        } else if (strcmp(command, "add") == 0) {
            char* nameBuffer = NULL;
            char* iataCode = getArgument(arguments, ':', &nameBuffer);
            if (iataCode == NULL || iataCode[0] == '\0') {
                printf("%s\n", errAddFormat);
                continue;
            }

            char* name = getArgument(nameBuffer, '\n', NULL);
            if (name == NULL || name[0] == '\0') {
                printf("%s\n", errAddFormat);
                continue;
            }

            rc = amAddAirport(manager, iataCode, name);
            switch (rc) {
            case AmSucsess:
                printf("Аэропорт '%s' добавлен в базу.\n", iataCode);
                break;
            case AmErrIataCodeIncorrect:
                printf("%s\n", errIataCodeIncorrect);
                break;
            case AmErrAirportAlreadyExist:
                printf("%s\n", errAirportAlreadyExist);
                break;
            case AmErrOnMalloc:
                printf("%s\n", errOnMalloc);
                break;
            case AmErrNoManager:
                printf("%s\n", errNoManager);
                break;
            default:
                printf("%s\n", errUnknown);
                break;
            }
        } else if (strcmp(command, "delete") == 0) {
            char* iataCode = getArgument(arguments, '\n', NULL);
            if (iataCode == NULL || iataCode[0] == '\0') {
                printf("%s\n", errNoCode);
                continue;
            }

            rc = amDeleteAirport(manager, iataCode);
            switch (rc) {
            case AmSucsess:
                printf("Аэропорт '%s' удалён из базы.\n", iataCode);
                break;
            case AmErrIataCodeIncorrect:
                printf("%s\n", errIataCodeIncorrect);
                break;
            case AmErrAirportNotFound:
                printf("Аэропорт с кодом '%s' не найден в базе.\n", iataCode);
                break;
            case AmErrNoManager:
                printf("%s\n", errNoManager);
                break;
            default:
                printf("%s\n", errUnknown);
                break;
            }
        } else if (strcmp(command, "save") == 0) {
            size_t savedCount = 0;
            rc = amSaveToFile(manager, path, &savedCount);
            switch (rc) {
            case AmSucsess:
                printf("База сохранена: %zu аэропортов.\n", savedCount);
                break;
            case AmErrNoFile:
                printf("%s (%s)\n", errNoFile, path);
                break;
            case AmErrNoManager:
                printf("%s\n", errNoManager);
                break;
            default:
                printf("%s\n", errUnknown);
                break;
            }
        } else {
            printf("%s (%s)\n", errUnknownCommand, command);
        }
    }

    amDeleteManager(&manager);
    return 0;
}
