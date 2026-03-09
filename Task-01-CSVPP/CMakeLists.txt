project(Task-01-CSVPP C)

# Console util
add_executable(csvpp csvpp.c CsvParser.h CsvParser.c)

# Tests
add_executable(csvpp_test test.c CsvParser.h CsvParser.c)
add_test(NAME CsvppTest COMMAND csvpp_test)
