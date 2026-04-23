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

## Formatting
```
find . -path ./build -prune -o -type f -name '*.[c|h]' -print | xargs clang-format-18 --style=file -i
```
