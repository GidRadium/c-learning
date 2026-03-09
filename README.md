# c-learning

## Install
```
git clone https://github.com/GidRadium/c-learning.git
cd c-learning
```

## Build
```
cmake . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build
```

## Start
```
./build/Example-task/example_main
./build/Task-01-CSVPP/csvpp "Task-01-CSVPP/test_data/input.csv" "Task-01-CSVPP/test_data/output.txt"
```

## Run tests
```
ctest --test-dir build --output-on-failure
```

## Formatting
```
find . -path ./build -prune -o -type f -name '*.[c|h]' -print | xargs clang-format-18 --style=file -i
```
