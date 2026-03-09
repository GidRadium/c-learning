#include "CsvParser.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LINE_LENGTH 4000
#define MAX_ROWS_COUNT 500
#define MAX_ELEMENTS_IN_ROW_COUNT 200

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

bool isNumber(const char *line, size_t startIndex, size_t length) {
    if (length == 0) {
        return false;
    }

    const char *start = line + startIndex;
    const char *end = start + length;

    char *parseEnd;
    strtod(start, &parseEnd);

    return (parseEnd != start && parseEnd == end);
}

CsvRow* getRow(const char *line) {
    if (line == NULL) {
        return NULL;
    }

    CsvRow *row = malloc(sizeof(CsvRow));
    size_t lineLength = strlen(line);
    row->line = malloc((lineLength + 1) * sizeof(char));
    strcpy(row->line, line);
    row->elements = malloc(MAX_ELEMENTS_IN_ROW_COUNT * sizeof(CsvElementInRow));
    row->elementsCount = 0;

    size_t i = 0;
    size_t start = 0;
    for (i = 0; i < lineLength; ++i) {
        if (row->line[i] == ',' || row->line[i] == '\n' || row->line[i] == '\0') {
            row->elements[row->elementsCount].startIndexInRow = start;
            row->elements[row->elementsCount].length = i - start;
            row->elements[row->elementsCount].isNumber = isNumber(row->line, start, i - start);
            row->elementsCount++;
            start = i + 1;
        }
    }

    return row;
}

void deleteRow(CsvRow *row) {
    if (row == NULL) {
        return;
    }

    if (row->elements != NULL) {
        free(row->elements);
    }

    if (row->line != NULL) {
        free(row->line);
    }

    free(row);
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

    size_t rowLength = parser->columnsCount * 3 + 1;
    size_t i = 0;
    for (i = 0; i < parser->columnsCount; ++i) {
        rowLength += parser->maxColumnsLength[i];
    }

    char *borderHead = malloc((rowLength + 1) * sizeof(char));
    memset(borderHead, '=', rowLength);
    borderHead[rowLength] = '\0';
    char *borderBody = malloc((rowLength + 1) * sizeof(char));
    memset(borderBody, '-', rowLength);
    borderBody[rowLength] = '\0';
    char *rowClean = malloc((rowLength + 1) * sizeof(char));
    memset(rowClean, ' ', rowLength);
    rowClean[rowLength] = '\0';
    char *rowTemp = malloc((rowLength + 1) * sizeof(char));

    size_t wallPosition = 0;
    for (i = 0; i < parser->columnsCount; ++i) {
        borderHead[wallPosition] = '+';
        borderBody[wallPosition] = '+';
        rowClean[wallPosition] = '|';
        wallPosition += parser->maxColumnsLength[i] + 3;
    }

    borderHead[wallPosition] = '+';
    borderBody[wallPosition] = '+';
    rowClean[wallPosition] = '|';

    fprintf(file, "%s\n", borderHead);

    size_t rowIndex = 0;
    for (rowIndex = 0; rowIndex < parser->rowsCount; ++rowIndex) {
        memcpy(rowTemp, rowClean, rowLength + 1);
        wallPosition = 2;

        for (i = 0; i < parser->columnsCount; ++i) {
            CsvElementInRow e = parser->rows[rowIndex].elements[i];

            memcpy(rowTemp + wallPosition + (e.isNumber ? parser->maxColumnsLength[i] - e.length : 0),
                parser->rows[rowIndex].line + e.startIndexInRow,
                e.length);

            wallPosition += parser->maxColumnsLength[i] + 3;
        }

        fprintf(file, "%s\n", rowTemp);

        if (rowIndex == 0) {
            fprintf(file, "%s\n", borderHead);
        } else {
            fprintf(file, "%s\n", borderBody);
        }
    }

    free(borderHead);
    free(borderBody);
    free(rowClean);
    free(rowTemp);
}

void deleteCsvParser(CsvParser *parser) {

}
