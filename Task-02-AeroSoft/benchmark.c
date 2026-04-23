#include "airport-manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** loadCodes(const char* path, size_t airportsNumber)
{
    if (airportsNumber == 0)
        return NULL;

    FILE* file = fopen(path, "r");
    if (file == NULL) {
        return NULL;
    }

    char** codes = (char**)calloc(airportsNumber, sizeof(char*));
    if (codes == NULL) {
        fclose(file);
        return NULL;
    }

    char lineBuffer[1024];
    char codeBuffer[4];
    char nameBuffer[1020];
    size_t count = 0;

    while (fgets(lineBuffer, sizeof(lineBuffer), file) && count < airportsNumber) {
        if (sscanf(lineBuffer, "%3[^:]:%255[^\n]", codeBuffer, nameBuffer) != 2) {
            continue;
        }

        codes[count] = (char*)malloc(4 * sizeof(char));
        if (codes[count] == NULL) {
            for (size_t i = 0; i < count; i++) {
                free(codes[i]);
            }

            free(codes);
            fclose(file);

            return NULL;
        }

        strncpy(codes[count], codeBuffer, 4);
        count++;
    }

    fclose(file);

    return codes;
}

void freeCodes(char** codes, size_t n)
{
    if (codes == NULL) {
        return;
    }

    for (size_t i = 0; i < n; i++) {
        free(codes[i]);
    }

    free(codes);
}

const char LETTERS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const size_t SIZE_LETTERS = sizeof(LETTERS) - 1;
char* setRandomIataCode(char* code)
{
    code[0] = LETTERS[rand() % SIZE_LETTERS];
    code[1] = LETTERS[rand() % SIZE_LETTERS];
    code[2] = LETTERS[rand() % SIZE_LETTERS];
    code[3] = '\0';

    return code;
}

void rushBenchmark(AirportManager* manager, char** codes, size_t n)
{
    char nameBuffer[1024];
    for (int i = 0; i < 50000; i++) {
        amFindAirport(manager, codes[rand() % n], nameBuffer, sizeof(nameBuffer));
    }
}

void updateBenchmark(AirportManager* manager, char** codes, size_t n)
{
    char* name = "KJNLKnkdjfnlksjdn flasddfn kak\0";
    char nameBuffer[1024];
    char codeBuffer[4];
    for (int i = 0; i < 10000; i++) {
        amAddAirport(manager, setRandomIataCode(codeBuffer), name);
        amFindAirport(manager, codes[rand() % n], nameBuffer, sizeof(nameBuffer));
    }
}

// ./exe rush|update path.txt
int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("Ошибка! Неверное количество аргументов.\n");
        return -1;
    }

    char* errUnknown = "Ошибка! Неожиданная ошибка!";
    char* errOnMalloc = "Ошибка! Не удалось выделить память.";
    char* errNoFile = "Ошибка! Не удалось прочитать файл.";
    char* errNoManager = "Ошибка! Менеджер не инициализирован.";

    const char* scenario = argv[1];
    const char* path = argv[2];

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

    size_t airportsCount = 0;
    rc = amInitFromFile(manager, path, &airportsCount);
    switch (rc) {
    case AmSucsess:
        // printf("Загружено %zu аэропортов. Система готова к работе.\n", airportsCount);
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

    char** codes = loadCodes(path, airportsCount);
    if (codes == NULL && airportsCount > 0) {
        printf("Ошибка загрузки кодов из файла.\n");
        amDeleteManager(&manager);
        return -1;
    }

    srand(42);
    if (strcmp(scenario, "rush") == 0) {
        rushBenchmark(manager, codes, airportsCount);
    } else if (strcmp(scenario, "update") == 0) {
        updateBenchmark(manager, codes, airportsCount);
    }

    freeCodes(codes, airportsCount);
    amDeleteManager(&manager);
}
