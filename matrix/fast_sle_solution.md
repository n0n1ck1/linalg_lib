# fast_sle_solution

### Функция, решающая систему линейных уравнений

#### Автор - Аня Ахматова [[AnyaAkhmatova](https://github.com/AnyaAkhmatova)]

Ответ может быть получен только в случае единственности решения.<br>
Иначе (если решений нет или их бесконечно много) функция возвращает 
пустую матрицу (`Matrix<T>(0, 0)`).

Кроме того, при выводе пустой матрицы выводится сообщение `Not a matrix`.

### Описание алгоритма
> Если левая и правая части системы разной длины, выкидывается 
> соответствующая ошибка.<br>
> Если длина левой части, меньше ее ширины, т.е. уравнений меньше, 
> чем неизвестных, то возвращается пустая матрица, так как в этом случае 
> либо нет решений, либо их бесконечно много.
> Иначе применяется алгоритм Гаусса решения СЛУ.
> 
> Алгоритм распараллеливается построчно.<br>
> Пусть `left_length` - количество строк в матричном виде СЛУ.<br>
> Создается `left_length` потоков, каждый из которых обрабатывает 
> свою строку с номером `id`.<br>
> Далее применяется прямой ход алгоритма, после - обратный.
>
> Используются:
> - `sequence` - массив размера `left_width`, он хранит порядок 
> строк, в котором они идут в ступенчатом виде 
> расширенной матрицы СЛУ;
> - `phase` - массив счетчиков, используемых для синхронизации
> этапов алгоритма в разных потоках, аналог `barrier`; 
> - `waiting` - вспомогательный массив счетчиков, используется 
> для сохранения правильного порядка выполнения операций при 
> прямом ходе алгоритма Гаусса;
> - переменные `inf_solution` и `do_not_have_solution` - 
> первая равна `true`, если у СЛУ уравнений меньше, 
> чем неизвестных, либо если после некоторых преобразований
> количество ненулевых строк (уравнений) стало меньше,
> чем количество неизвестных;<br>
> вторая равна `true`, если система противоречива и не имеет
> решений.

> Прямой ход:
> Строка `id` пытается занять позицию pos в массиве 
> sequence при условии, что элемент с номером `pos`
> в строке `id` не равен нулю и позиция pos в массиве 
> sequence еще не занята другой строкой.<br>
> Если строке это удается, то она заняла свое место (`pos`) 
> в ступенчатом виде расширенной матрицы СЛУ, она
> готова к "нормировке" и обратному ходу.<br>
> Если строке это не удается, то она обязана дождаться, 
> когда какая-то другая строка займет позицию `pos` 
> в массиве `sequence`, после чего она должна занулить 
> у себя элемент с номером `pos`, затем продолжить попытки
> занять свое место в `sequence`.<br>
> Здесь используется массив `waiting`, который в ячейке `pos` 
> хранит количество строк, ожидающих, когда будет кем-то
> занята ячейка `pos` в массиве `sequence`. В случае, если 
> сумма потоков, окончивших прямой ход, и потоков, 
> находящихся в состоянии ожидания, равна количеству 
> потоков всего, т.е. `phase[0] + waiting[pos] == left_length`,
> утверждается, что ни одна строка не смогла занять позицию `pos`
> в ступенчатом виде расширенной матрицы СЛУ, а значит решений
> либо бесконечно много, либо их нет, значение `inf_solution` 
> устанавливается `true`, в конце возвращается пустая матрица.
> 
> Если `inf_solution` и `do_not_have_solution` до сих пор равны 
> `false`, значит ступенчатый вид матрицы готов и можно 
> переходить к следующему этапу.
> 
> Если строка заняла место в `sequence`, то ее нужно нормировать
> по первому ненулевому элементу - элементу с номером `pos`.<br>
> Иначе нужно проверить, что строка состоит полностью из нулей.<br>
> Левая часть такой строки нулевая, так как она занулилась при
> попытках попасть в `sequence`. Если правая часть не нулевая,
> значение `do_not_have_solution` устанавливается `true`, в конце 
> возвращается пустая матрица.

> Обратный ход:
> Если `inf_solution` и `do_not_have_solution` до сих пор равны 
> `false`, значит получен ступенчатый вид матрицы с единицами
> на главной диагонали и установлено, что решение единственно.<br>
> Строка `id` зануляет элементы с номерами от `pos + 1` до
> `left_width - 1`, начиная с конца. `pos` - место строки в 
> ступенчатом виде. К этому моменту все элементы до `pos` в строке id
> уже равны нулю. Аналогично прямому ходу, чтобы не нарушить 
> порядок выполнения операций и не добавить ничего лишнего, 
> для обнуления `i`-ого элемента при помощи строки с номером 
> `sequence[i]` строка `id` должна дождаться, когда
> строки с номерами `sequence[i]`...`sequence[left_width - 1]`
> закончат обработку. Это проверяется с помощью `phase[2]`.

После всего вышеописанного, левая часть расширенной матрицы имеет
улучшенный ступенчатый вид. Достаточно вернуть правую часть 
расширенной матрицы СЛУ.


### Реализация алгоритма

```cpp
template<typename T>
Matrix<T> fast_sle_solution(const Matrix<T>& left_part, const Matrix<T>& right_part) {
  int left_length = left_part.GetLength();
  int left_width = left_part.GetWidth();
  int right_length = right_part.GetLength();
  int right_width = right_part.GetWidth();
  if (left_length != right_length) {
    throw std::length_error("Shapes do not match");
  }
  int length = left_width;
  int width = right_width;
  Matrix<T> sle_matrix = concatenate(left_part, right_part, 1);
  Matrix<T> answer(length, width);
  std::vector<std::atomic<int>> sequence(left_width);
  for (size_t i = 0; i < size_t(left_width); ++i) {
    sequence[i].store(-1);
  }
  std::vector<std::atomic<int>> phase(3);
  for (size_t i = 0; i < size_t(3); ++i) {
    phase[i].store(0);
  }
  std::vector<std::atomic<int>> waiting(left_width);
  for (size_t i = 0; i < size_t(left_width); ++i) {
    waiting[i].store(0);
  }
  std::atomic<bool> do_not_have_solution{ false };
  std::atomic<bool> inf_solution{ false };
  std::vector<std::thread> threads;
  if (left_length < left_width) {
    inf_solution.store(true);
  }
  else {
    threads.reserve(left_length);
    for (int i = 0; i < left_length; ++i) {
      threads.emplace_back([&](int id) {
        int pos = 0;
        int needed_val = -1;
        while (pos < left_width) {
          if (std::abs(sle_matrix(id, pos)) > 1e-10 && sequence[pos].compare_exchange_weak(needed_val, id)) {
            break;
          }
          waiting[pos].fetch_add(1);
          while (phase[0].load() < pos + 1) {
            std::this_thread::yield();
            if (phase[0].load() + waiting[pos].load() == left_length && phase[0].load() < pos + 1) {
              inf_solution.store(true);  // inf or no solution
              break;
            }
          }
          if (inf_solution.load()) {
            break;
          }
          sle_matrix.row_addition(id, sequence[pos].load(),
                                  -sle_matrix(id, pos) / sle_matrix(sequence[pos].load(), pos));
          ++pos;
          needed_val = -1;
        }
        phase[0].fetch_add(1);
        while (phase[0].load() != left_length) {
          std::this_thread::yield();
        }
        if (!inf_solution.load() && !do_not_have_solution.load()) {
          if (pos == left_width) {
            for (int j = 0; j < left_width + right_width &&
                            !inf_solution.load() && !do_not_have_solution.load(); ++j) {
              if (std::abs(sle_matrix(id, j)) > 1e-10) {
                do_not_have_solution.store(true);  // no solution
                break;
              }
            }
          }
          else {
            sle_matrix.row_multiplication(id, 1 / sle_matrix(id, pos));
          }
        }
        phase[1].fetch_add(1);
        while (phase[1].load() != left_length) {
          std::this_thread::yield();
        }
        if (!inf_solution.load() && !do_not_have_solution.load() && pos < left_width) {
          for (int j = left_width - 1; j > pos; --j) {
            while (phase[2].load() < left_width - j) {
              std::this_thread::yield();
            }
            sle_matrix.row_addition(id, sequence[j].load(), -sle_matrix(id, j));
          }
          phase[2].fetch_add(1);
          for (int j = 0; j < width; ++j) {
            answer(pos, j) = sle_matrix(id, left_width + j);
          }
        }
      }, i);
    }
    for (auto& t : threads) {
      t.join();
    }
  }
  if (inf_solution.load() || do_not_have_solution.load()) {
    return Matrix<T>(0, 0);
  }
  return answer;
}
```