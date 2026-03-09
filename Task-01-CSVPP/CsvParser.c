#include "CsvParser.h"
#include <stdio.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 4000

typedef struct CsvElementInRow {
    size_t startIndexInRow;
    size_t size;
    bool isNumber;
} CsvElementInRow;

typedef struct CsvRow {
    char *line;
    size_t elementsCount;
    CsvElementInRow *elements;

} CsvRow;

CsvRow* getRow();
void deleteRow(CsvRow *row);

typedef struct CsvParser CsvParser;

CsvParser* createFromFile(FILE *file) {
    return NULL;
}

void writeToFile(CsvParser *parser, FILE *file) {

}

void deleteCsvParser(CsvParser *parser) {

}
