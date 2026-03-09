#pragma once

#include <stdio.h>

typedef struct CsvParser CsvParser;

CsvParser* createFromFile(FILE *file);

void writeToFile(CsvParser *parser, FILE *file);

void deleteCsvParser(CsvParser *parser);
