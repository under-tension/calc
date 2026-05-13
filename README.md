Консольный калькулятор. Калькулятор поддерживает следующие действия: сложение, вычитание, умножение, деление и возведение в степень.

dev-зависимости:
1) clang-tidy >= v19
2) clang-format >= v19

Сборка проекта
```
cmake -B build
cmake --build build
cmake --build build --target install
```

Сборка проекта с запуском статического анализатора и форматера
```
cmake -B build -DENABLE_CLANG_FORMAT=ON -DENABLE_CLANG_TIDY=ON
cmake --build build
cmake --build build --target install
```

Для дебага
```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
```

Пример использования
```
./calc {\"val1\": 2, \"val2\": 4, \"operation\": \"+\"}
```

```
./calc {\"val1\": 2, \"val2\": 4, \"operation\": \"^\"}
```


```
./calc {\"val1\": 3, \"operation\": \"\!\"}
```
