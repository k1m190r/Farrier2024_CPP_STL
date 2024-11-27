#include <algorithm>
#include <chrono>
#include <execution>
#include <functional>
#include <iterator>
#include <numeric>
#include <print>
#include <ranges>
#include <vector>

using std::vector, std::print, std::string;
namespace R = std::ranges;
namespace V = std::ranges::views;

int main() {
  /************************************************************************************/
  // 01_itoa.cpp
  {
    const int N_time_steps = 20;

    double time_step_delta = 0.01;
    auto scale = [time_step_delta](auto x) { return x * time_step_delta; };
    auto time_steps = V::iota(0, N_time_steps) | V::transform(scale) | R::to<vector>();

    print("Ex01: {}\n", time_steps);
  }

  /************************************************************************************/
  // 02_accumulate.cpp
  {
    vector<string> words = {"Hello", ", ", "world", "!"};

    using tp = const string &;
    auto concat = [](tp x, tp y) { return x + y; };
    string concatenated = R::fold_left(words, string(""), concat);

    print("Ex02: concatenated: {}\n", concatenated);
  }

  /************************************************************************************/
  // 03_adjacentdifference.cpp
  {
    using VD = vector<double>;

    VD values = {8.0, 16.0, 64.0, 256.0, 4096.0};
    VD ratios(values.size());

    auto div = [](auto x, auto y) { return (x == 0.0) ? 0.0 : y / x; };
    std::adjacent_difference(values.begin(), values.end(), ratios.begin(), div);

    print("Ex03: Ratios between consecutive elements: {}\n", ratios);
  }

  /************************************************************************************/
  // 04_parallelsum.cpp
  {
    using clock = std::chrono::high_resolution_clock;
    using std::chrono::duration;

    const int N = 1'000'000;
    auto numbers = V::iota(1, N);

    auto start = clock::now();
    auto sum = R::fold_left(numbers, 0, std::plus<>());
    auto end = clock::now();
    duration<double> elapsed = end - start;

    print(" ∑ {} {}\n", sum, elapsed);

    start = clock::now();
    int par_sum = std::reduce(std::execution::par, numbers.begin(), numbers.end());
    end = clock::now();
    elapsed = end - start;

    print("p∑ {} {}\n", par_sum, elapsed);
  }

  /************************************************************************************/
  // 05_parallelreduce.cpp
  {
    const int N = 1'000;
    auto numbers = V::iota(1, N);

    namespace exe = std::execution;
    auto sq = [](auto x) { return x * x; };
    auto par_sum_of_sq = std::transform_reduce(exe::par, numbers.begin(), numbers.end(),
                                               0, std::plus<double>(), sq);

    print("Parallel Sum of Squares: {}\n", par_sum_of_sq);
  }

  /************************************************************************************/
  // 06_sortedranges.cpp
  {
    std::vector<int> d = {10, 20, 30, 40, 50, 60, 70, 80, 90};
    int tgt = 40;
    auto lb = std::lower_bound(d.begin(), d.end(), tgt);
    auto ub = std::upper_bound(d.begin(), d.end(), tgt);

    bool exists = std::binary_search(d.begin(), d.end(), tgt);

    std::vector<int> set1 = {10, 20, 30, 40, 50};
    std::vector<int> set2 = {30, 40, 50, 60, 70};
    std::vector<int> intersection(std::min(set1.size(), set2.size()));

    auto it = std::set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(),
                                    intersection.begin());

    std::vector<int> difference(std::max(set1.size(), set2.size()));

    auto diffEnd = std::set_difference(set1.begin(), set1.end(), set2.begin(),
                                       set2.end(), difference.begin());
    bool isSorted = std::is_sorted(d.begin(), d.end());

    print("Lower Bound: {}\n", std::distance(d.begin(), lb));
    print("Upper Bound: {}\n", std::distance(d.begin(), ub));
    print("Exists: {}\n", exists);

    print("Intersection: ");
    for (auto i = intersection.begin(); i != it; ++i) print("{} ", *i);
    print("\n");

    print("Difference: ");
    for (auto i = difference.begin(); i != diffEnd; ++i) print("{} ", *i);
    print("\n");

    print("Is Sorted: {}\n", isSorted);
  }
  {
    vector<int> d = {10, 20, 30, 40, 50, 60, 70, 80, 90};
    int tgt = 40;

    auto lb = R::lower_bound(d, tgt);
    auto ub = R::upper_bound(d, tgt);

    bool exists = R::binary_search(d, tgt);

    vector<int> set1{10, 20, 30, 40, 50}, set2{30, 40, 50, 60, 70};

    vector<int> intersection;
    intersection.reserve(std::min(set1.size(), set2.size()));

    using std::back_inserter;
    R::set_intersection(set1, set2, back_inserter(intersection));

    std::vector<int> difference;
    difference.reserve(std::max(set1.size(), set2.size()));

    R::set_difference(set1, set2, back_inserter(difference));
    bool isSorted = std::is_sorted(d.begin(), d.end());

    print("Lower Bound: {}\n", std::distance(d.begin(), lb));
    print("Upper Bound: {}\n", std::distance(d.begin(), ub));
    print("Exists: {}\n", exists);

    print("Intersection: {}\n", intersection);
    print("Difference: {}\n", difference);
    print("Is Sorted: ", isSorted);
  }
}