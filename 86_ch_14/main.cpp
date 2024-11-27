#include <algorithm>
#include <print>
#include <random>
#include <ranges>
#include <vector>

using std::vector, std::print, std::string;
namespace R = std::ranges;
namespace V = std::ranges::views;

int main() {
  // partition
  {
    print("\npartition\n");

    vector v{7, 12341, 3, 13, 9, 242341};
    print("original: {}\n", v);

    auto b = v.begin(), e = v.end();
    auto is_even = [](auto n) { return n % 2 == 0; };
    auto tail = R::partition(v, is_even).begin();
    print("part 0: {}\n", R::subrange(b, tail));
    print("part rest: {} {}\n", R::subrange(tail, e), R::subrange(tail, e).size());
  }

  // permutations
  {
    vector d{1, 2, 3};
    print("\npermunations\n");
    do {
      print("{}\n", d);
    } while (R::next_permutation(d).found);
  }

  // shuffle
  {
    vector n{1, 2, 3, 4, 5};
    print("\nshuffle\n");

    std::random_device rd;
    std::mt19937 gen{rd()};

    for (auto i : V::iota(1, 10)) {
      R::shuffle(n, gen);
      print("{}: {}\n", i, n);
    }
  }

  // rotate
  {
    print("\nrotate\n");
    vector n{1, 2, 3, 4, 5};
    R::rotate(n, n.begin() + 2);
    print("\n{} rotate 2\n", n);
  }

  // heap
  {
    vector h = {1, 6, 1, 8, 0, 3};
    print("start:     {}\n", h);

    R::make_heap(h);
    print("make_heap: {}\n", h);

    h.push_back(9);
    print("push 9:    {}\n", h);

    R::push_heap(h);
    print("push_heap: {} is_heap {} \n", h, R::is_heap(h));

    R::sort_heap(h);
    print("sort_heap: {} is_sorted {} \n", h, R::is_sorted(h));
  }
}
