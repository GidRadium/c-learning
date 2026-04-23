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
python3 Task-02-AeroSoft/data/prepare.py
mv airports.txt Task-02-AeroSoft/data/airports.txt
./build/Task-02-AeroSoft/aerosoft Task-02-AeroSoft/data/airports.txt
```

## Test
```
ctest --test-dir build --output-on-failure
```

## Benchmark
```
rm -rf build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_FLAGS="-O2 -g"
cmake --build build

perf stat ./build/Task-02-AeroSoft/benchmark-avl rush Task-02-AeroSoft/data/airports.txt
perf stat ./build/Task-02-AeroSoft/benchmark-list rush Task-02-AeroSoft/data/airports.txt
perf stat ./build/Task-02-AeroSoft/benchmark-avl update Task-02-AeroSoft/data/airports.txt
perf stat ./build/Task-02-AeroSoft/benchmark-list update Task-02-AeroSoft/data/airports.txt

perf record ./build/Task-02-AeroSoft/benchmark-list rush Task-02-AeroSoft/data/airports.txt
perf report
```

## Formatting
```
find . -path ./build -prune -o -type f -name '*.[c|h]' -print | xargs clang-format-18 --style=file -i
```
