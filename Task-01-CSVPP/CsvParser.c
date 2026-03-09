#include "CsvParser.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 4000
#define MAX_ROWS_COUNT 500

typedef struct CsvElementInRow {
    size_t startIndexInRow;
    size_t length;
    bool isNumber;
} CsvElementInRow;

typedef struct CsvRow {
    char *line;
    size_t elementsCount;
    CsvElementInRow *elements;

} CsvRow;

CsvRow* getRow(const char *line) {
    return NULL;
}

void deleteRow(CsvRow *row) {

}

typedef struct CsvParser {
    size_t rowsCount;
    CsvRow *rows;
    size_t columnsCount;
    size_t *maxColumnsLength;
} CsvParser;

CsvParser* createFromFile(FILE *file) {
    if (file == NULL) {
        return NULL;
    }

    char line[MAX_LINE_LENGTH];
    if (!fgets(line, sizeof(line), file)) {
        return NULL;
    }

    CsvRow *row = getRow(line);
    if (row == NULL) {
        return NULL;
    }

    CsvParser *parser = malloc(sizeof(CsvParser));
    parser->columnsCount = row->elementsCount;
    parser->maxColumnsLength = malloc(parser->columnsCount * sizeof(size_t));
    parser->rowsCount = 1;
    parser->rows = malloc(MAX_ROWS_COUNT * sizeof(CsvRow));
    parser->rows[0] = *row;
    free(row);
    row = NULL;

    size_t i = 0;
    for (i = 0; i < parser->columnsCount; ++i) {
        parser->maxColumnsLength[i] = parser->rows[0].elements[i].length;
    }

    while (fgets(line, sizeof(line), file) && parser->rowsCount < MAX_ROWS_COUNT) {
        row = getRow(line);
        if (row->elementsCount != parser->columnsCount) {
            deleteRow(row);
            deleteCsvParser(parser);
            return NULL;
        }

        for (i = 0; i < parser->columnsCount; ++i) {
            if (parser->maxColumnsLength[i] < row->elements[i].length) {
                parser->maxColumnsLength[i] = row->elements[i].length;
            }
        }

        parser->rows[parser->rowsCount] = *row;
        free(row);
        row = NULL;
        parser->rowsCount++;
    }

    return parser;
}

void writeToFile(CsvParser *parser, FILE *file) {
    if (file == NULL || parser == NULL) {
        return;
    }
}

void deleteCsvParser(CsvParser *parser) {

}
