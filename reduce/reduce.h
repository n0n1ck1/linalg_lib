#pragma once

#include <cstddef>


template <typename RandomAccessIterator, class T, class Func>
T parallel_reduce(RandomAccessIterator /*begin*/, RandomAccessIterator /*end*/,
                  const T& /*initial_value*/, Func /*func*/, size_t /*n_threads*/) {
  return T{};
}

