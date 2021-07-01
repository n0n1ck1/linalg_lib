# parallel-cpp
## Задачи
| Название           | День | Баллы  |
| ------------------ |:----:| ------:|
| is_prime           |  0   |    -/0 |
| hello_world        |  1   |    -/1 |
| deadlock           |  1   |    -/1 |
| philosophers       |  1   |    -/1 |
| reduce             |  1   |    -/1 |
| thread_safe_queue  |  2   |    -/1 |
| thread_safe_vector |  2   |    -/1 |
| unbuffered_channel |  2   |    -/2 |
| spin_lock          |  3   |    -/1 |
| ticket_lock        |  3   |    -/1 |
| mcs_lock           |  3   |    -/1 |
| rw_spin_lock       |  3   |    -/2 |
| mpsc_queue         |  4   |    -/2 |
| mpmc_queue         |  4   |    -/2 |
| Сумма              |  -   |   0/17 |



## Как начать работать?

Для начала необходимо установить `ssh`, `git`, `cmake` и `clang`:
### Ubuntu
`$ sudo apt update && sudo apt install ssh git cmake clang`
### MacOS
`$ brew install ssh git cmake`

### SSH
Для удобства стоит сгенерировать ssh ключ и добавить его на GitHub, чтобы не вводить каждый раз логин и пароль.  
Инструкция по [генерации](https://docs.github.com/en/github/authenticating-to-github/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent) и [добавлению](https://docs.github.com/en/github/authenticating-to-github/connecting-to-github-with-ssh/adding-a-new-ssh-key-to-your-github-account) и [проверке](https://docs.github.com/en/github/authenticating-to-github/connecting-to-github-with-ssh/testing-your-ssh-connection).

### Копирование репозитория
GitHub не дает сделать сделать приватный fork от публичного репозитория, поэтому нужно будет "перенести" все файлы.  
1. Создайте репозиторий с названием parallel-cpp у себя на GitHub. На этапе создания сделайте его приватным, больше ничего выставлять не нужно.
2. Скачайте репозиторий:
```
$ git clone --bare git@github.com:maxkuzn/parallel-cpp.git
```
3. Сделайте mirror-push:
```
$ cd parallel-cpp.git
$ git push --mirror git@github.com:<your_login>/parallel-cpp.git
```
4. Удалите репозиторий:
```
$ cd ..
$ rm -rf parallel-cpp.git
```
5. Скачайте свой репозиторий:
```
$ git clone git@github.com:<your_login>/parallel-cpp.git
```

### Организационное
В настройках репозитория на сайте добавите в коллаборацию пользователя `maxkuzn`.
Также заполните [форму](https://docs.google.com/forms/d/e/1FAIpQLSfz9bU1kAajYbmtqr2ZISsQpPqiNDBUHx0u1uSkAG454AxZkg/viewform?usp=sf_link).


### Сдача задач
1. Создать ветку с решением для конкретной задачи (название ветки должно содержать название задачи):
```
$ git checkout -b is_prime
```
2. Решить задачу.
3. Добавить измененные файлы в коммит (например, для задачи is_prime):
```
$ git add is_prime/is_prime.h
```
4. Создать коммит:
```
$ git commit -m "Some informative message"
```
5. "Залить" коммит на GitHub, создав ветку:
```
$ git push --set-upstream origin is_prime
```
6. Сделать pull request на сайте - нажать большую кнопу "Compare & pull request". Далее добавить в reviewers пользователя `maxkuzn` и нажать "Create pull request". **НЕ НАЖИМАТЬ** кнопку "Merge pull request".


Если хотите изменить решение, нужно сделать следующее:
1. Переключиться на нужную ветку
```
$ git checkout is_prime
```
2. Выполнить пункты 2 и 3 выше
3. "Залить" коммит на GitHub:
```
$ git push
```

Заметьте, что переключаться между ветками можно в том случае, если вы не меняли никаких файлов, либо все изменения закоммичены. Если вы по ошибке начали в одной ветке менять решение, а дальше решили изменить ветку, то можно сделать `stash` - он сохраняет и прячет все изменения, а дальше эти изменения можно наложить на новую ветку. Параметр `-b` нужен, если вы создаете новую ветку. Если переключаетесь на существующую, то этот флаг не нужен.
```
$ git stash
$ git checkout [-b] branch_name
$ git stash pop
```

Подробнее про ветки в git можно почитать [здесь](https://www.atlassian.com/git/tutorials/using-branches/git-checkout) и [здесь](https://www.atlassian.com/git/tutorials/using-branches).


### Сборка
```
$ mkdir build
$ cd build
$ [СС="clang" CXX="clang++"] cmake ..
$ make [-j [N]]
```
CMake собирает информацию о проекте и подготавливает правила компиляции и линковки для компилятора. `СС="clang" CXX="clang++"` необходимо писать в Ubuntu для того, чтобы проект собирался именно clang, а не gcc. В MacOS используется clang по умолчанию.  
Make непосредственно компилирует и собирает проект. Параметр `-j [N]` говорит `make` о том, что проект должен собираться параллельно. Вместо `N` необходимо указать количество параллельных работ (или ничего не указывать для максимальной параллельности).  
**Make необходимо запускать после изменения файлов и перед запуском тестов. СMake достаточно запустить один раз.**

Также можно собрать проект с address или thread санитайзером (для ASAN аналогично):
```
$ mkdir tsan_build
$ cd tsan_build
$ [СС="clang" CXX="clang++"] cmake -DTSAN ..
$ make [-j [N]]
```

## Запуск тестов
**Тесты не являются гарантией того, что задача будет засчитана на полный балл. Они лишь помогают убедиться в том, что ваше решение хоть насколько-то верное.**

Для запуска всех тестов из директории `build` нужно выполнить команду `ctest`:
```
$ cd build
$ ctest
```

Чтобы запустить тесты конкретной задачи, нужно выполнить соответствующий файл.  
Например, для задачи `is_prime`:
```
$ cd build
$ ./is_prime/is_prime
```
Или же можно запустить конкретный тест:  
`$ ./is_prime/is_prime --gtest-filter="IsPrime.One"`  
Или:  
`$ ./is_prime/is_prime --gtest-filter="*One"` 

### ASAN и TSAN
Тесты проверяют, что ваш код выдает ожидаемый результат, однако они не проверяют, например, наличие data race в программе. Для этого есть ASAN и TSAN сборки.  
Чтобы убедиться, что вы все сделали верно, запустите тесты еще и для этих двух сборок. Запуск тестов аналогичен, но запускать их нужно из директорий `asan_build` и `tsan_build`.  
Обратите внимание, что в этих сборках код может работать в несколько раз медленнее - это нормально.

### Timeout
У каждого теста есть timeout, после которого кидается исключение с текстом "Timeout!". Если вы считаете, что все сделали правильно, но тест не проходит по времени, попробуйте увеличить/убрать этот лимит в файле `test.cpp` у соответствующего теста.  
Для ASAN и TSAN сборок timeout'ы выключены.
