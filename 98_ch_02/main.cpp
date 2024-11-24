#include <algorithm>
#include <fmt/ranges.h>
#include <forward_list>
#include <iostream> // cout
#include <iterator>
#include <list>
#include <mutex>
#include <ranges>
#include <string_view>
#include <thread>
#include <vector>

using std::forward_list;
using std::string, std::string_view;
using std::vector, fmt::print, fmt::format;
namespace ranges = std::ranges;

using std::generate_n, std::back_inserter;

// MAIN

int main() {

  // 01_inputiterator.cpp
  {
    vector<int> numbers = {10, 20, 30, 40, 50};

    print("Ex01: ");
    for (auto it = numbers.begin(); it != numbers.end(); ++it) {
      std::cout << *it << " ";
    }
    std::cout << "\n";
  }

  // 02_outputiterator.cpp
  {
    vector<int> numbers;

    generate_n(back_inserter(numbers), 10, [n = 0]() mutable { return ++n; });

    print("Ex02: {}\n", numbers);
  }

  // 03_forwarditerator.cpp
  {
    forward_list<int> flist = {10, 20, 30, 40, 50};
    print("Ex03: original flist {}\n", flist);

    ranges::for_each(flist, [](auto &x) { ++x; });
    print("Ex03: modified flist {}\n", flist);
  }

  // 04_bidirectionaliterator.cpp
  {
    std::list<int> numbers = {1, 2, 3, 4, 5};

    print("Ex04: original list {}\n", numbers);
    print("Ex04: reverse list {}\n", std::views::reverse(numbers));
  }

  // 05_randomaccessiterator.cpp
  {
    std::mutex vecMutex;
    using IV = std::vector<int>;

    auto add_to_vector = [&vecMutex](IV &numbers, int value) {
      std::lock_guard<std::mutex> guard(vecMutex);
      numbers.push_back(value);
    };

    auto print_vector = [&vecMutex](IV &numbers) {
      std::lock_guard<std::mutex> guard(vecMutex);
      print("Ex5: {}\n", numbers);
    };

    std::vector<int> numbers;
    std::thread t1(add_to_vector, std::ref(numbers), 1);
    std::thread t2(add_to_vector, std::ref(numbers), 2);
    t1.join();
    t2.join();
    std::thread t3(print_vector, std::ref(numbers));
    t3.join();
  }

  // 06_nonmemberbeginend.cpp
  {

    auto display = [](auto &x) { print("{}\n", x); };

    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::list<int> lst = {6, 7, 8, 9, 10};
    std::array<int, 5> arr = {11, 12, 13, 14, 15};

    print("Vector: ");
    display(vec);

    print("List: ");
    display(lst);

    print("Array: ");
    display(arr);
  }

  // 07_squareiterator.cpp
  {
    class SquareIterator {
    public:
      //   using iterator_category = std::random_access_iterator_tag;
      //   using reference = int &;
      using value_type = int;
      using difference_type = std::ptrdiff_t;
      using pointer = int *;

      explicit SquareIterator(pointer ptr) : ptr(ptr) {}

      value_type operator*() const {
        auto x = *ptr;
        return x * x;
      }
      pointer operator->() { return ptr; }

      SquareIterator &operator++() {
        ++ptr;
        return *this;
      }

      SquareIterator operator++(int) {
        SquareIterator tmp = *this;
        ++ptr;
        return tmp;
      }

      SquareIterator &operator+=(difference_type diff) {
        ptr += diff;
        return *this;
      }
      SquareIterator operator+(difference_type diff) const {
        return SquareIterator(ptr + diff);
      }

      value_type operator[](difference_type diff) const {
        auto x = *(ptr + diff);
        return x * x;
      }

      bool operator!=(const SquareIterator &other) const {
        return ptr != other.ptr;
      }

    private:
      pointer ptr;
    };

    std::vector<int> vec = {1, 2, 3, 4, 5};
    SquareIterator begin(vec.data());
    SquareIterator end(vec.data() + vec.size());

    print("Ex07: ");
    for (auto it = begin; it != end; ++it) {
      std::cout << *it << ' ';
    }

    SquareIterator it = begin + 2;
    print("\n @ 2: {}", *it);
    print("\n @ 3: {}\n", it[1]);
  }

  // 08_alphaiterator.cpp
  {
    struct RGBA {
      uint8_t r, g, b, a;
    };

    class AlphaIterator {
    public:
      using iterator_category = std::input_iterator_tag;
      using value_type = uint8_t;
      using difference_type = std::ptrdiff_t;
      using pointer = uint8_t *;
      using reference = uint8_t &;

      explicit AlphaIterator(std::vector<RGBA>::iterator itr) : itr_(itr) {}

      AlphaIterator() = default;

      reference operator*() { return itr_->a; }

      AlphaIterator &operator++() {
        ++itr_;
        return *this;
      }

      AlphaIterator operator++(int) {
        AlphaIterator tmp(*this);
        ++itr_;
        return tmp;
      }

      bool operator==(const AlphaIterator &other) const {
        return itr_ == other.itr_;
      }

      bool operator!=(const AlphaIterator &other) const {
        return itr_ != other.itr_;
      }

    private:
      std::vector<RGBA>::iterator itr_;
    };

    std::vector<RGBA> bitmap = {
        {255, 0, 0, 128}, {0, 255, 0, 200}, {0, 0, 255, 255},
        {255, 0, 0, 127}, {0, 255, 0, 201}, {0, 0, 255, 25},
    };

    auto b = AlphaIterator(bitmap.begin());
    auto e = AlphaIterator(bitmap.end());
    print("Ex08: Alpha values via iterator before: {}\n",
          fmt::join(b, e, ", "));

    std::for_each(b, e, [](auto &x) { ++x; });
    print("Ex08: Alpha values via iterator after : {}\n",
          fmt::join(b, e, ", "));

    auto r = std::ranges::subrange(b, e);
    print("Ex08: use subrage {}\n", r);
  }
}
