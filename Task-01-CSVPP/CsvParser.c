#include "CsvParser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 4000
#define MAX_ROWS_COUNT 4000
#define MAX_ELEMENTS_IN_ROW_COUNT 200

typedef struct CsvElementInRow {
    size_t startIndexInRow;
    size_t length;
    bool isNumber;
} CsvElementInRow;

typedef struct CsvRow {
    char* line;
    size_t elementsCount;
    CsvElementInRow* elements;

} CsvRow;

bool isNumber(const char* line, size_t startIndex, size_t length)
{
    if (length == 0) {
        return false;
    }

    const char* start = line + startIndex;
    const char* end = start + length;

    char* parseEnd;
    strtod(start, &parseEnd);

    return (parseEnd != start && parseEnd == end);
}

void deleteRow(CsvRow* row)
{
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

void deleteRowContent(CsvRow* row)
{
    if (row == NULL) {
        return;
    }

    if (row->elements != NULL) {
        free(row->elements);
    }

    if (row->line != NULL) {
        free(row->line);
    }
}

CsvRow* getRow(const char* line)
{
    if (line == NULL) {
        return NULL;
    }

    CsvRow* row = malloc(sizeof(CsvRow));
    if (row == NULL) {
        return NULL;
    }

    size_t lineLength = strlen(line);
    row->line = malloc((lineLength + 1) * sizeof(char));
    if (row->line == NULL) {
        deleteRow(row);
        return NULL;
    }

    memcpy(row->line, line, lineLength + 1);
    row->elements = malloc(MAX_ELEMENTS_IN_ROW_COUNT * sizeof(CsvElementInRow));
    if (row->elements == NULL) {
        deleteRow(row);
        return NULL;
    }

    row->elementsCount = 0;

    size_t i = 0;
    size_t start = 0;
    size_t quotesCount = 0;

    for (i = 0; i < lineLength; ++i) {
        if (row->line[i] == '"') {
            quotesCount++;
        }

        if ((row->line[i] == ',' || row->line[i] == '\n' || row->line[i] == '\0') && quotesCount % 2 == 0) {
            row->elements[row->elementsCount].startIndexInRow = start;
            row->elements[row->elementsCount].length = i - start;
            row->elements[row->elementsCount].isNumber = isNumber(row->line, start, i - start);
            row->elementsCount++;
            start = i + 1;
        }
    }

    return row;
}

typedef struct CsvParser {
    size_t rowsCount;
    CsvRow* rows;
    size_t columnsCount;
    size_t* maxColumnsLength;
} CsvParser;

CsvParser* createFromFile(FILE* file)
{
    if (file == NULL) {
        return NULL;
    }

    char line[MAX_LINE_LENGTH];
    if (!fgets(line, sizeof(line), file)) {
        return NULL;
    }

    CsvRow* row = getRow(line);
    if (row == NULL) {
        return NULL;
    }

    CsvParser* parser = malloc(sizeof(CsvParser));
    if (parser == NULL) {
        deleteRow(row);
        return NULL;
    }

    parser->maxColumnsLength = malloc(row->elementsCount * sizeof(size_t));
    if (parser->maxColumnsLength == NULL) {
        deleteCsvParser(parser);
        deleteRow(row);
        return NULL;
    }

    parser->rows = malloc(MAX_ROWS_COUNT * sizeof(CsvRow));
    if (parser->rows == NULL) {
        deleteCsvParser(parser);
        deleteRow(row);
    }

    parser->columnsCount = row->elementsCount;
    parser->rowsCount = 1;
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

void writeToFile(CsvParser* parser, FILE* file)
{
    if (file == NULL || parser == NULL) {
        return;
    }

    size_t rowLength = parser->columnsCount * 3 + 1;
    size_t i = 0;
    for (i = 0; i < parser->columnsCount; ++i) {
        rowLength += parser->maxColumnsLength[i];
    }

    char* borderHead = malloc((rowLength + 1) * sizeof(char));
    char* borderBody = malloc((rowLength + 1) * sizeof(char));
    char* rowClean = malloc((rowLength + 1) * sizeof(char));
    char* rowTemp = malloc((rowLength + 1) * sizeof(char));

    if (borderHead == NULL || borderBody == NULL || rowClean == NULL || rowTemp == NULL) {
        if (borderHead != NULL) {
            free(borderHead);
        }

        if (borderBody != NULL) {
            free(borderBody);
        }

        if (rowClean != NULL) {
            free(rowClean);
        }

        if (rowTemp != NULL) {
            free(rowTemp);
        }

        return;
    }

    memset(borderHead, '=', rowLength);
    memset(borderBody, '-', rowLength);
    memset(rowClean, ' ', rowLength);

    borderHead[rowLength] = '\0';
    borderBody[rowLength] = '\0';
    rowClean[rowLength] = '\0';

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

void deleteCsvParser(CsvParser* parser)
{
    if (parser == NULL) {
        return;
    }

    if (parser->maxColumnsLength != NULL) {
        free(parser->maxColumnsLength);
    }

    if (parser->rows != NULL) {
        size_t i = 0;
        for (i = 0; i < parser->rowsCount; ++i) {
            deleteRowContent(&parser->rows[i]);
        }

        free(parser->rows);
    }

    free(parser);
}
