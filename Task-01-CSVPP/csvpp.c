#include "CsvParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ./build/Task-01-CSVPP/csvpp "Task-01-CSVPP/test_data/input.csv" "Task-01-CSVPP/test_data/output.txt"
int main(int argc, char* argv[])
{
    if (argc != 3) {
        fprintf(stderr, "ERROR. Usage: %s <input.csv> <output.txt>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], argv[2]) == 0) {
        fprintf(stderr, "ERROR. Can't rewrite same file %s\n", argv[1]);
        return 1;
    }

    FILE* inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        fprintf(stderr, "ERROR. Can't open file %s\n", argv[1]);
        return 1;
    }

    FILE* outputFile = fopen(argv[2], "w");
    if (outputFile == NULL) {
        fprintf(stderr, "ERROR. Can't open file %s\n", argv[2]);
        fclose(inputFile);
        return 1;
    }

    CsvParser* csvpp = createFromFile(inputFile);
    fclose(inputFile);

    if (csvpp == NULL) {
        fprintf(stderr, "ERROR. Input file is incorrect.\n");
        fclose(outputFile);
        return 1;
    }

    writeToFile(csvpp, outputFile);
    fclose(outputFile);
    deleteCsvParser(csvpp);
    return 0;
}
