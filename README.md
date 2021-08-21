# linalg_lib

## О библиотеке

В linalg_lib реализованы все основные операции на матрицах,
встречающиеся на первом курсе линейной алгебры.


Для тестов используется библиотека [gtest](https://github.com/google/googletest),
для бенчмарок - [googlebenchmark](https://github.com/google/benchmark).


### Над проектом работали:

- Стас Жухлистов [[n0n1ck1](https://github.com/n0n1ck1)]
- Анна Ахматова [[AnyaAkhmatova](https://github.com/AnyaAkhmatova)]
- Матвей Каиров [[thebensky](https://github.com/thebensky)]


### Структура репозитория

```
linalg_lib
│
├── CMakeLists.txt
├── README.md
│
├── benchmarks
│   │
│   │  // бенчмарки для сравнения работы
│   │  // последовательных и параллельных алгоритмов
│   │
│   ├── CMakeLists.txt
│   └── matrix_benchmark.cpp
│
├── matrix
│   │  
│   ├── CMakeLists.txt
│   ├── README.md
│   ├── functions.h             // основная библиотека
│   ├── matrix.cpp
│   ├── matrix.h                // файл с классом Matrix<>
│   └── sequential_functions.h  // последовательные функции
│
└── tests
    │
    ├── CMakeLists.txt
    ├── test_matrix.cpp      // тесты основных функций
    ├── test_sequential.cpp  // тесты последовательных функций
    └── util
        ├── ...
```



## Начало работы

Создайте матрицу:

```cpp
Matrix<int> rectangle(/* число строк */ 6, /* число столбцов */ 11);

Matrix<int> square(/* сторона квадратной матрицы */ 6);

Matrix<int> from_vector(/* двумерный вектор */ {{1, 2, 3},
                                                {4, 5, 6}});
```

Используйте матричные операторы:
```cpp
// ^ - матричное умножение
Matrix<int> product = (square ^ rectangle);

// + - сложение матриц
product += rectangle;

// * - умножение матрицы на скаляр
product *= 2;

// либо на другую матрицу такого же размера поэлементно
product *= product
```

Доступны функции:
- `transposed(Matrix)` (транспонирование матрицы)
- `det(Matrix)` (определитель квадратной матрицы)
- `inverse(Matrix)` (обратная матрица)
- `rank(Matrix)` (ранг матрицы)
- и другие!

