## Reduce

В данной задаче необходимо реализовать параллельный аналог функции [`std::reduce`](https://en.cppreference.com/w/cpp/algorithm/reduce). В отличии от стандартной версии, ваша реализация должна принимать количество нитей для параллельного вычисления результата.  

---

Не нужно использовать `mutex`, достаточно одной переменной [`atomic`](https://en.cppreference.com/w/cpp/atomic/atomic/compare_exchange).  
Также остерегайтесь [false sharing](https://dzone.com/articles/false-sharing#:~:text=False%20sharing%20is%20a%20term,execution%20in%20an%20SMP%20system.).
