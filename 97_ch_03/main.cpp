#include <algorithm>
#include <chrono>
#include <cstddef>
#include <fmt/ranges.h>
#include <iterator>
#include <string>
#include <string_view>
#include <vector>

using std::generate_n, std::back_inserter;
using std::string, std::string_view;
using std::vector, fmt::print;

// MAIN

// 05_customallocator.cpp
template <typename T> struct CustomAllocator {
  // public:
  using value_type = T;

  CustomAllocator() noexcept {}

  template <typename U> CustomAllocator(const CustomAllocator<U> &) noexcept {}

  T *allocate(std::size_t n) {
    print("    alloc n {} of size {} bytes.\n", n, sizeof(T));
    return static_cast<T *>(::operator new(n * sizeof(T)));
  }

  void deallocate(T *p, std::size_t) noexcept {
    print("    delete.\n");
    ::operator delete(p); // global delete
  }

  template <typename U, typename... Args> void construct(U *p, Args &&...args) {
    print("    constructing \n");
    new (p) U(std::forward<Args>(args)...);
  }

  template <typename U> void destroy(U *p) {
    print("    destroying \n");
    p->~U();
  }
};

int main() {

  // 01_sizevscapacity.cpp
  {
    using std::generate_n, std::back_inserter;

    vector<int> myVec;
    print("Ex01: init size {} cap {}\n", myVec.size(), myVec.capacity());

    generate_n(back_inserter(myVec), 10, [n = 0] mutable { return ++n; });

    print("Ex01 after push: {} size {} cap {}\n", myVec, myVec.size(),
          myVec.capacity());

    myVec.resize(5);
    print("Ex01 after resize(5): {} size {} cap {}\n", myVec, myVec.size(),
          myVec.capacity());

    myVec.shrink_to_fit();
    print("Ex01 after shrink_to_fit(): {} size {} cap {}\n", myVec,
          myVec.size(), myVec.capacity());

    myVec.push_back(555);
    print("Ex01 after push_back(555): {} size {} cap {}\n", myVec, myVec.size(),
          myVec.capacity());
  }

  // 02_resizevscapacity.cpp
  {
    vector<int> numbers = {1, 2, 3, 4, 5};

    auto printVectorDetails = [&numbers](std::string_view sv) {
      print("Ex02 numbers {}: {} size {} cap {}\n", sv, numbers, numbers.size(),
            numbers.capacity());
    };

    printVectorDetails("init");

    numbers.resize(8);
    printVectorDetails("after resizing to 8");

    numbers.resize(3);
    printVectorDetails("after resizing to 3");
  }

  // 03_reserve.cpp
  {
    using clock = std::chrono::high_resolution_clock;
    using std::chrono::duration;

    constexpr size_t N = 1'000'000;

    // #################################################################################

    vector<int> numbers1;
    auto start1 = clock::now();

    generate_n(back_inserter(numbers1), N, [n = 0] mutable { return n++; });

    auto end1 = clock::now();

    duration<double> elapsed1 = end1 - start1;
    print("Ex03: Elapsed 1 {}\n", elapsed1.count());

    // #################################################################################

    vector<int> numbers2;
    numbers2.reserve(N); // Reserve memory upfront.

    auto start2 = clock::now();
    generate_n(back_inserter(numbers2), N, [n = 0] mutable { return n++; });

    auto end2 = clock::now();

    duration<double> elapsed2 = end2 - start2;
    print("Ex03: Elapsed 2 {}\n", elapsed2.count());
  }

  // 04_shrinktofit.cpp
  {
    vector<int> numbers;
    numbers.reserve(100);

    print("Ex04: {} size {} cap {}\n", numbers, numbers.size(),
          numbers.capacity());

    generate_n(back_inserter(numbers), 10, [n = 0] mutable { return ++n; });

    print("Ex04 after 10: {} size {} cap {}\n", numbers, numbers.size(),
          numbers.capacity());

    numbers.shrink_to_fit();

    print("Ex04: {} size {} cap {}\n", numbers, numbers.size(),
          numbers.capacity());
  }

  // 05_customallocator.cpp
  {
    std::vector<int, CustomAllocator<int>> numbers;

    print("Ex05: Pushing back numbers 1 to 5:\n");
    generate_n(back_inserter(numbers), 5, [n = 0] mutable { return ++n; });

    print("\nEx05: Clearing the vector:\n");
    numbers.clear();
  }
}
