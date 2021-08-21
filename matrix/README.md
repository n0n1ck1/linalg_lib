# Список доступных функций

### Конструкторы:

| Header                                              |                    Требования к входным данным                    |
|-----------------------------------------------------|:-----------------------------------------------------------------:|
| `Matrix()`                                          |                                 -                                 |
| `Matrix(const std::vector<std::vector<T>>& matrix)` | `matrix[i].size() == matrix[j].size() ∀ 0 < i, j < matrix.size()` |
| `Matrix(std::vector<std::vector<T>>&& matrix)`      |                               — // —                              |
| `Matrix(const size_t& h, const size_t& w)`          |                                 -                                 |
| `Matrix(const size_t& n)`                           |                                 -                                 |
| `Matrix(const Matrix& other)`                       |                                 -                                 |
| `Matrix(Matrix&& other)`                            |                                 -                                 |


### Встроенные функции:

| Header                                                                                                                                                     | Описание                                                                                                                                          | Требования к входным данным                                                    |
|------------------------------------------------------------------------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------------------------------------------------------------|:------------------------------------------------------------------------------:|
| `size_t GetWidth()`                                                                                                                                        | Возвращает число столбцов                                                                                                                         | -                                                                              |
| `size_t GetLength()`                                                                                                                                       | Возвращает число строк                                                                                                                            | -                                                                              |
| `std::pair<size_t, size_t> GetShape()`                                                                                                                     | Возвращает пару `{length, width}`                                                                                                                 | -                                                                              |
| `Matrix get_row(const size_t& row)`                                                                                                                        | Принимает номер строки,<br>возвращает строку `row`                                                                                                | `row < length`                                                                 |
| `Matrix get_column(const size_t& column)`                                                                                                                  | Принимает номер столбца,<br>возвращает столбец `column`                                                                                           | `column < width`                                                               |
| `Matrix get_submatrix(const size_t& start_row, const size_t& end_row,`<br>`const size_t& start_column, const size_t& end_column)`                          | Принимает границы подматрицы,<br>возвращает заданную подматрицу                                                                                   | ограничения по размеру                                                         |
| `Matrix& concatenate(const Matrix& other, size_t axis=0)`                                                                                                  | Принимает матрицу для конкатенации и направление,<br>возвращает объединенные матрицы<br>(`axis=0` -> по вертикали,<br>`axis=1` -> по горизонтали) | `axis == 0` -> одинаковое число столбцов `axis == 1` -> одинаковое число строк |
| `bool empty()`                                                                                                                                             | Возвращает `true`, если матрица не задана,<br>иначе возвращает `false`                                                                            | -                                                                              |
| `Matrix& row_addition(size_t i, size_t j, T k),`<br>`Matrix& row_multiplication(size_t i, T k),`<br>`Matrix& row_switching(size_t i, size_t j)`            | Элементарные преобразования над строками                                                                                                          | ограничения по размеру                                                         |
| `Matrix& column_addition(size_t i, size_t j, T k),`<br>`Matrix& column_multiplication(size_t i, T k),`<br>`Matrix& column_switching(size_t i, size_t j)`   | Элементарные преобразования над столбцами                                                                                                         | ограничения по размеру                                                         |
| `void transpose()`                                                                                                                                         | Транспонирование матрицы                                                                                                                          | -                                                                              |
| `void fill_random(const T& range_low, const T& range_high)`                                                                                                | Заполняет матрицу случайными значениями между range_low и range_high                                                                              | -                                                                              |


### Операторы

| Оператор                                                                                                                       | Описание                      |                Требования к входным данным               |
|--------------------------------------------------------------------------------------------------------------------------------|-------------------------------|:--------------------------------------------------------:|
| `Matrix<T> operator+(const Matrix<T>& matrix1, const Matrix<T>& matrix2)`                                                      | Сложение матриц               |                  Матрицы одного размера                  |
| `Matrix<T> operator-(const Matrix<T>& matrix1, const Matrix<T>& matrix2)`                                                      | Вычитание матриц              |                        --- // ---                        |
| `Matrix<T> operator*(const Matrix<T>& matrix1, const Matrix<T>& matrix2)`                                                      | Поэлементное умножение матриц |                        --- // ---                        |
| `Matrix<T> operator/(const Matrix<T>& matrix1, const Matrix<T>& matrix2)`                                                      | Поэлементное деление матриц   |                        --- // ---                        |
| `Matrix<T> operator*(const T& scale, const Matrix<T>& matrix)`, `Matrix<T> operator*(const Matrix<T>& matrix, const T& scale)` | Умножение матрицы на скаляр   |                             -                            |
| `Matrix<T> operator^(const Matrix<T>& matrix1, const Matrix<T>& matrix2)`                                                      | Умножение двух матриц         | Число столбцов в `matrix1` равно числу строк в `matrix2` |
| `std::ostream& operator<<(std::ostream& out, const Matrix<T>& matrix)`                                                         | Вывод матрицы в поток `out`   |                             -                            |


### Внешние функции

| Header                                                                                                        | Описание                                                                                |              Требования к входным данным             |
|---------------------------------------------------------------------------------------------------------------|-----------------------------------------------------------------------------------------|:----------------------------------------------------:|
| `Matrix<T> dot(const Matrix<T>& left, const Matrix<T>& right)`                                                | Матричное умножение                                                                     |                 аналогично встроенной                |
| `Matrix<T> concatenate(const Matrix<T>& matrix1, const Matrix<T>& matrix2, size_t axis=0)`                    | Конкатенация матриц                                                                     |                 аналогично встроенной                |
| `Matrix<T> diag(const T& elem, const size_t& size)`                                                           | Возвращает диагональную матрицу размера `size` с `elem` на диагонали                    |                           -                          |
| `Matrix<T> diag_from_vector(const std::vector<T> vector)`                                                     | Возвращает диагональную матрицу с элементами `vector` на диагонали                      |                           -                          |
| `Matrix<T> random_matrix(const size_t& h, const size_t& w, const T& range_low=0.0, const T& range_high=1.0)`  | Возвращает матрицу случайных величин от `range_low` до `range_high` размера `h` на `w`  |                           -                          |
| `T det(Matrix<T> matrix)`                                                                                     | Возвращает определитель матрицы                                                         |                  квадратная матрица                  |
| `Matrix<T> inverse(const Matrix<T>& matrix)`                                                                  | Возвращает матрицу, обратную данной                                                     |                  квадратная матрица                  |
| `Matrix<T> transposed(const Matrix<T> &matrix)`                                                               | Транспонирует `matrix`                                                                  |                           -                          |
| `Matrix<T> sle_solution(const Matrix<T>& left_part, const Matrix<T>& right_part)`                             | Решает СЛУ из `left_part` и `right_part`                                                |  одинаковое число строк в `left_part` и `right_part  |
| `Matrix<T> fast_sle_solution(const Matrix<T>& left_part, const Matrix<T>& right_part)`                        | Решает СЛУ ([объяснение работы алгоритма](./fast_sle_solution.md))                      |                         —//—                         |
| `size_t rank(Matrix<T> matrix)`                                                                               | Возвращает ранг матрицы                                                                 |                           -                          |
| `size_t fast_rank(Matrix<T> matrix)`                                                                          | Возвращает ранг матрицы (работает аналогично `fast_sle_solution`)                       |                           -                          |
