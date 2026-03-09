#include <stdio.h>
#include <stdlib.h>

// ./build/Task-01-CSVPP/csvpp "Task-01-CSVPP/test_data/input.csv" "Task-01-CSVPP/test_data/output.txt"
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "ERROR. Usage: %s <input.csv> <output.txt>\n", argv[0]);
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        fprintf(stderr, "ERROR. Can't open file %s\n", argv[1]);
        return 1;
    }



    fclose(inputFile);

}
