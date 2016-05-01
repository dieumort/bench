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
void insertion_sort(RAIter first, RAIter last) {
  // less
  using value_type = typename std::iterator_traits<RAIter>::value_type;
  const auto & less = std::less<value_type>();

  for (auto it_i = first + 1; it_i != last; ++it_i) {
    for (auto it_j = it_i; it_j != first && less(*it_j, *(it_j - 1)); --it_j) {
      std::swap(*it_j, *(it_j - 1));
    }
  }
}

//------------------------------------------------------------------------------

// quick sort
template <typename RAIter>
void quick_sort(RAIter first, RAIter last) {
  // less
  using value_type = typename std::iterator_traits<RAIter>::value_type;
  const auto & less = std::less<value_type>();

  // small range
  const auto distance = std::distance(first, last);
  if (distance <= TH_QSORT) {
    insertion_sort(first, last);
    return;
  }

  // find pivot
  auto mid = first + (distance / 2);

  // begin > mid
  if (less(*mid, *first)) {
    std::swap(*first, *mid);
  }
  // begin > end - 1
  if (less(*(last - 1), *first)) {
    std::swap(*first, *(last - 1));
  }
  // mid > end - 1
  if (less(*(last - 1), *mid)) {
    std::swap(*mid, *(last - 1));
  }

  const auto pivot = *mid;
  auto left = first + 1;
  auto right = last - 2;
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
  quick_sort(first, right + 1);
  quick_sort(left, last);
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
  const size_t trials = (argc > 2) ? std::stoi(argv[2]) : 1;

  // generate randomly-shuffled integers
  auto start = std::chrono::steady_clock::now();
  std::vector<int> vec(size);
  std::iota(vec.begin(), vec.end(), 0);
  auto ans(vec);
  std::shuffle(vec.begin(), vec.end(), std::mt19937(std::random_device{}()));
  const auto t_gen = get_time(start);

  // my sort
  auto t_my_sort = 0.0;
  for (size_t i = 0; i < trials; i++) {
    // copy
    auto vec_copy(vec);

    // sort
    start = std::chrono::steady_clock::now();
    quick_sort(vec_copy.begin(), vec_copy.end());
    t_my_sort += get_time(start);

    // check
    if (i == 0 && ans != vec_copy) {
      std::cerr << "sort error" << std::endl;
      return 1;
    }
  }

  // std::sort
  auto t_std_sort = 0.0;
  for (size_t i = 0; i < trials; i++) {
    // copy
    auto vec_copy(vec);

    // sort
    start = std::chrono::steady_clock::now();
    std::sort(vec_copy.begin(), vec_copy.end());
    t_std_sort += get_time(start);
  }

  // print
  std::cout << boost::format("%10d %12.9f %12.9f %12.9f") % vec.size()
      % t_gen % (t_my_sort / trials) % (t_std_sort / trials) << std::endl;

  return 0;
}

//------------------------------------------------------------------------------
