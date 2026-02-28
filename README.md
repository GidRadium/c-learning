# c-learning

## Install
```
git clone https://github.com/GidRadium/c-learning.git
cd c-learning
```

## Build
```
cmake --build build
```

## Formatting
```
find . -path ./build -prune -o -type f -name '*.[c|h]' -print | xargs clang-format-18 --style=file -i
```
