#include <algorithm>
#include <print>
#include <ranges>
#include <vector>

using std::vector, std::print, std::string;
namespace R = std::ranges;
namespace V = std::ranges::views;

int main() {
  /************************************************************************************/
  // 01_ranges.cpp
  {
    vector data = {5, 2, 9, 1, 5, 6, 8, 7, 3, 4};
    print("data: {}\n", data);

    auto flt = V::filter;
    auto tx = V::transform;
    auto is_even = [](auto x) { return x % 2 == 0; };
    auto neg = [](auto x) { return -x; };
    auto even_sorted = data | flt(is_even) | tx(neg) | R::to<vector>();
    R::sort(even_sorted);

    print("sorted even: {}\n", even_sorted);
  }

  /************************************************************************************/
  // 02_chaining.cpp
  {
    auto is_prime = [](auto n) {
      if (n <= 1) return false;

      for (auto i = 2; i * i <= n; i++)
        if (n % i == 0) return false;

      return true;
    };

    vector nums = {4, 6, 8, 9, 10, 11, 13, 15, 17, 19, 23, 25};

    auto flt = V::filter;
    auto gt_10 = [](auto n) { return n > 10; };
    auto prime_gt_10 = nums | flt(gt_10) | flt(is_prime) | V::take(3);

    print("First three prime numbers greater than 10: {}\n", prime_gt_10);
  }
}