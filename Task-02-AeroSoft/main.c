#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "airport-manager.h"


char* getArgument(char* string, char stopSymbol, char** otherArguments)
{
    if (string == NULL) {
        (*otherArguments) = string;
        return NULL;
    }

    size_t length = strlen(string);
    size_t start = 0;
    while (start < length && string[start] == ' ') {
        start++;
    }

    size_t end = start;
    while (end < length && string[end] != stopSymbol) {
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

int main (int argc, char* argv[])
{
    if (argc != 2) {
        printf("Error! Incorrect arguments count.\n");
        return -1;
    }

    char* errUnknown = "Error! Unexpected error!";
    char* errOnMalloc = "Error! Can't allocate memory.";
    char* errNoFile = "Error! Can't read file.";

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
            return -1;
        case AmErrOnMalloc:
            printf("%s\n", errOnMalloc);
            return -1;
        default:
            printf("%s\n", errUnknown);
            return -1;
    }

    while (true) {
        char buffer[512];

        printf("> ");
        fflush(stdout);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error! Can't read query.\n");
            break;
        }

        char* arguments = NULL;
        char* command = getArgument(buffer, ' ', &arguments);
        if (command == NULL) {
            printf("Error! Unknown command.\n");
            continue;
        }

        if (strcmp(command, "quit") == 0) {
            break;
        } else if (strcmp(command, "find") == 0) {
            char* iataCode = getArgument(arguments, '\n', NULL);
            if (iataCode == NULL) {
                printf("Error! No IATA code passed.\n");
                continue;
            }
        } else if (strcmp(command, "add") == 0) {
            char* nameBuffer = NULL;
            char* iataCode = getArgument(arguments, ':', &nameBuffer);
            if (iataCode == NULL) {
                printf("Error! No IATA code passed.\n");
                continue;
            }

            char* name = getArgument(nameBuffer, '\n', NULL);
            if (name == NULL) {
                printf("Error! No name passed.\n");
                continue;
            }

        } else if (strcmp(command, "delete") == 0) {
            char* iataCode = getArgument(arguments, '\n', NULL);
            if (iataCode == NULL) {
                printf("Error! No IATA code passed.\n");
                continue;
            }

        } else if (strcmp(command, "save") == 0) {
            break;
        } else {
            printf("Error! Unknown command.\n");
        }
    }

    amDeleteManager(&manager);
    return 0;
}
