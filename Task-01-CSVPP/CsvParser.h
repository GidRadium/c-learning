#pragma once

#include <stdio.h>

// Represents .csv file as .txt table.
typedef struct CsvParser CsvParser;

// Reads data from .csv to the memmory.
CsvParser* createFromFile(FILE* file);

// Writes table to text file.
void writeToFile(CsvParser* parser, FILE* file);

// Frees all allocated memmory.
void deleteCsvParser(CsvParser* parser);
