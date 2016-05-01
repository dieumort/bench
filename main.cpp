//------------------------------------------------------------------------------
/**
 * @file main.cpp
 */
//------------------------------------------------------------------------------

#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <numeric>
#include <functional>
#include <algorithm>
#include <boost/format.hpp>

//------------------------------------------------------------------------------

// constants
enum {
  TH_QSORT = 32,
};

//------------------------------------------------------------------------------

// insertion sort
template <typename RAIter>
void insertion_sort(RAIter begin, RAIter end) {
  // less
  using value_type = typename std::iterator_traits<RAIter>::value_type;
  const auto & less = std::less<value_type>();

  for (auto it_i = begin + 1; it_i != end; ++it_i) {
    for (auto it_j = it_i; it_j != begin && less(*it_j, *(it_j - 1)); --it_j) {
      std::swap(*it_j, *(it_j - 1));
    }
  }
}

//------------------------------------------------------------------------------

// quick sort
template <typename RAIter>
void quick_sort(RAIter begin, RAIter end) {
  // less
  using value_type = typename std::iterator_traits<RAIter>::value_type;
  const auto & less = std::less<value_type>();

  // small range
  const auto distance = std::distance(begin, end);
  if (distance <= TH_QSORT) {
    insertion_sort(begin, end);
    return;
  }

  // find pivot
  auto mid = begin + (distance / 2);

  // begin > mid
  if (less(*mid, *begin)) {
    std::swap(*begin, *mid);
  }
  // begin > end - 1
  if (less(*(end - 1), *begin)) {
    std::swap(*begin, *(end - 1));
  }
  // mid > end - 1
  if (less(*(end - 1), *mid)) {
    std::swap(*mid, *(end - 1));
  }

  const auto pivot = *mid;
  auto left = begin + 1;
  auto right = end - 2;
  while (true) {
    // left
    while (less(*left, pivot)) {
      ++left;
    }

    // right
    while (less(pivot, *right)) {
      --right;
    }

    if (left >= right) {
      break;
    }

    std::swap(*left++, *right--);
  }

  // recursive
  quick_sort(begin, right + 1);
  quick_sort(left, end);
}

//------------------------------------------------------------------------------

// time
double get_time(const std::chrono::steady_clock::time_point & start) {
  using namespace std::chrono;
  return duration<double>(steady_clock::now() - start).count();
}

//------------------------------------------------------------------------------

int main(int argc, char * argv[]) {
  // parameters
  const size_t size = (argc > 1) ? std::stoi(argv[1]) : 1;

  // generate randomly-shuffled integers
  auto start = std::chrono::steady_clock::now();
  std::vector<int> vec(size);
  std::iota(vec.begin(), vec.end(), 0);
  std::shuffle(vec.begin(), vec.end(), std::mt19937(std::random_device{}()));
  const auto t_gen = get_time(start);

  // copy
  start = std::chrono::steady_clock::now();
  auto vec_c(vec);
  const auto t_copy = get_time(start);

  // sort
  start = std::chrono::steady_clock::now();
  quick_sort(vec.begin(), vec.end());
  const auto t_sort = get_time(start);

  // std::sort
  start = std::chrono::steady_clock::now();
  std::sort(vec_c.begin(), vec_c.end());
  const auto t_sort_c = get_time(start);

  // check & print
  if (vec == vec_c) {
    std::cout << boost::format("%10d %12.9f %12.9f %12.9f %12.9f")
        % vec.size() % t_gen % t_copy % t_sort % t_sort_c << std::endl;
  }

  return 0;
}

//------------------------------------------------------------------------------
