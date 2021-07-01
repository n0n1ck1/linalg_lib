## Hello, World!

В этой задаче вам предстоит написать "Hello, World!" из нескольких нитей. Конструктор лишь принимает число нитей, а метод `SayHello` должен создавать необходимое число нитей, каждая из которых пишет в переменную `os` сообщение в формате `"Hello, World! From thread <thread_id>\n"`.  

---

Так как `ostream` не является потокобезопасным (thread-safe), то доступ к нему нужно ограничить через [`mutex`](https://en.cppreference.com/w/cpp/thread/mutex).  
Для получения id текущей нити нужно вызвать функцию [`get_id`](https://en.cppreference.com/w/cpp/thread/get_id) из стандартной библиотеки.
