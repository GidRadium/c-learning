#include "CsvParser.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    // Test
    assert(createFromFile(NULL) == NULL);

    // Test
    deleteCsvParser(NULL);

    // Test on correct data
    FILE* input = tmpfile();
    assert(input != NULL);
    fprintf(input, "name,age\nAlice,30\n");
    rewind(input);

    CsvParser* parser = createFromFile(input);
    assert(parser != NULL);

    FILE* output = tmpfile();
    assert(output != NULL);
    writeToFile(parser, output);

    fseek(output, 0, SEEK_END);
    assert(ftell(output) > 0);

    fclose(input);
    fclose(output);
    deleteCsvParser(parser);

    // Test on incorrect data
    FILE* bad = tmpfile();
    assert(bad != NULL);
    fprintf(bad, "a,b,c\n1,2\n");
    rewind(bad);
    assert(createFromFile(bad) == NULL);
    fclose(bad);

    return 0;
}
