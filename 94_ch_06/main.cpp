#include <algorithm>
#include <array>
#include <cctype>
#include <chrono>
#include <deque>
#include <execution>
#include <format>
#include <functional>
#include <list>
#include <numeric>
#include <print>
#include <random>
#include <ranges>
#include <vector>

using std::print, std::format, std::string;

int main() {
  /***********************************************************************************/
  // 01_array.cpp
  {
    struct Point {
      int x{0}, y{0};
    };

    constexpr auto sumArray = [](const std::array<int, 5> &arr) {
      int sum = 0;
      for (const auto &val : arr) sum += val;
      return sum;
    };

    constexpr auto sumArray_alg = [](const std::array<int, 5> &arr) {
      return std::ranges::fold_left(arr, 0, std::plus<int>());
    };

    std::array<int, 5> numbers = {5, 3, 8, 1, 4};
    std::array<Point, 3> points = {{{1, 2}, {3, 4}, {5, 6}}};

    // structured bindings with &[x, y]
    for (const auto &[x, y] : points) print("({}, {})\n", x, y);

    constexpr std::array<int, 5> constNumbers = {1, 2, 3, 4, 5};

    constexpr int totalSum = sumArray(constNumbers);
    constexpr int sum_fold = sumArray_alg(constNumbers);
    print("\nEx1: Compile-time sum of array elements: {} {}\n", totalSum, sum_fold);

    std::sort(numbers.begin(), numbers.end());
    print("\nEx1: Sorted numbers: {} \n", numbers);

    int searchFor = 3;
    if (std::find(numbers.begin(), numbers.end(), searchFor) != numbers.end())
      print("\nEx1: Found {}  in the array.\n", searchFor);
    else
      print("\nEx1: Didn't find {} in the array.\n", searchFor);
  }

  /***********************************************************************************/
  // 02_vector.cpp
  {
    class Employee {
     public:
      Employee(int _id, const string &_name) : id(_id), name(_name) {}

      int getId() const { return id; }
      const string &getName() const { return name; }

      void setName(const string &newName) { name = newName; }

     private:
      int id{0};
      string name;
    };

    std::vector<Employee> employees;

    employees.reserve(5);
    employees.emplace_back(1, "Lisa");
    employees.emplace_back(2, "Corbin");
    employees.emplace_back(3, "Aaron");
    employees.emplace_back(4, "Amanda");
    employees.emplace_back(5, "Regan");

    auto vtx = std::views::transform;
    auto p_emp = [](const auto &e) {
      return format("{:2d}:{}", e.getId(), e.getName());
    };
    print("Ex2: start {}\n", employees | vtx(p_emp));

    // id=3 -> Chuck
    auto is_id_3 = [](const auto &e) { return e.getId() == 3; };
    // auto it = std::find_if(employees.begin(), employees.end(), is_id_3);
    auto it = std::ranges::find_if(employees, is_id_3);

    if (it != employees.end()) it->setName("Chuck");

    print("Ex2: after mod: {}\n", employees | vtx(p_emp));

    // erase id==2
    auto is_id_2 = [](const auto &e) { return e.getId() == 2; };
    // employees.erase(std::remove_if(employees.begin(), employees.end(), is_id_2),
    //                 employees.end());
    std::erase_if(employees, is_id_2);

    print("Ex2: after erase: {}\n", employees | vtx(p_emp));
  }

  /***********************************************************************************/
  // 03_vector_bool.cpp
  {
    std::vector<bool> boolVec = {true, false, true, true, false};

    boolVec[1] = true;
    print("Ex03: boolVec[1] {}", boolVec[1]);

    auto ref = boolVec[1];
    ref = false;

    print("Ex03: Second element after modifying copy: {}\n", boolVec[1]);
    print("Ex03: {}\n", boolVec);

    // Pushing values
    boolVec.push_back(false);

    // Resizing
    boolVec.resize(10, true);

    // Capacity and size
    print("Ex03: Size: {}, Capacity: {}\n", boolVec.size(), boolVec.capacity());

    // Clearing the vector
    boolVec.clear();
    print("Ex03: after clear() Size: {}, Capacity: {}\n", boolVec.size(),
          boolVec.capacity());
  }

  /***********************************************************************************/
  // 04_deque.cpp
  {
    // demonstrate using a deque as a sliding window over data.
    auto processInSlidingWindow = [](const std::deque<int> &data, size_t windowSize) {
      for (size_t i = 0; i <= data.size() - windowSize; ++i) {
        // int sum = 0;
        // for (size_t j = i; j < i + windowSize; ++j) {
        //   sum += data[j];
        // }

        // auto sum = std::accumulate(
        // data.begin() + i, data.begin() + i + windowSize, 0);
        // seq = std::execution::seq;

        auto sum = std::reduce(std::execution::par, data.begin() + i,
                               data.begin() + i + windowSize);

        print("Ex04: Average of window starting at index {} : {}\n", i,
              static_cast<double>(sum) / windowSize);
      }
    };

    std::deque<int> numbers;

    for (int i = 1; i <= 5; ++i) {
      numbers.push_back(i * 10);    // 10, 20, ..., 50
      numbers.push_front(-i * 10);  // -10, -20, ..., -50
    }

    print("Ex04: Numbers in deque: {}\n", numbers);

    numbers.pop_front();
    numbers.pop_back();

    print("Ex04: After removing front and back: {}\n", numbers);

    processInSlidingWindow(numbers, 3);

    auto tx = [](int n) { return n * 2; };
    std::transform(numbers.begin(), numbers.end(), numbers.begin(), tx);

    print("Ex04: After doubling each element: {}\n", numbers);
  }

  // accumulate, reduce (seq and parallel), and all the folds
  // https://en.cppreference.com/w/cpp/algorithm/reduce
  // https://biowpn.github.io/bioweapon/2024/04/23/accumulate-reduce-fold.html
  {
    using clock = std::chrono::high_resolution_clock;
    using std::chrono::duration;

    // const auto &policy,
    auto measure = [](std::vector<int> v) {
      const auto start = clock::now();

      auto sum = 0;
      for (auto x : v) sum += x;

      //   auto sum = std::reduce(policy, v.begin(), v.end());
      //   auto sum = std::accumulate(v.begin(), v.end(), 0);
      //   auto sum = std::ranges::fold_left(v, 0, std::plus<int>());

      //   auto make_double = [](auto x) { return (double)x; };
      //   auto op = std::plus<double>{};
      //   auto sum = std::transform_reduce(v.begin(), v.end(), 0, op, make_double);

      const auto end = clock::now();
      duration<double> elapsed = end - start;
      print("{} {}\n", sum, elapsed);
    };

    std::vector<int> v(1'000'000);
    std::mt19937 gen{std::random_device{}()};
    std::ranges::generate(v, gen);

    measure(v);
    // namespace exe = std::execution;
    // measure(exe::seq, v);
    // measure(exe::unseq, v);
    // measure(exe::par_unseq, v);
    // measure(exe::par, v);
  }

  /***********************************************************************************/
  // 05_list.cpp
  {
    // auto display = [](auto &lst) { print("Ex05: {}\n", lst); };

    std::list<int> numbers = {5, 1, 8, 3, 7};

    print("Ex05 init: {}\n", numbers);
    print("Ex05 reverse: {}\n", numbers | std::views::reverse);

    // auto at_8 = std::ranges::find_if(numbers, [](auto x) { return x == 8; });
    auto at_8 = std::ranges::find(numbers, 8);
    print("Ex05 from 8 till end: {}\n", std::ranges::subrange(at_8, numbers.end()));

    auto pos = std::ranges::find(numbers, 8);
    numbers.insert(pos, 2112);
    print("Ex05 insert after 8: {}\n", numbers);

    numbers.sort();
    std::list<int> more_numbers = {2, 6, 4};
    more_numbers.sort();
    numbers.merge(more_numbers);
    print("Ex05 after sort and merge: {}\n", numbers);

    std::list<int> additional_numbers = {99, 100, 101};
    numbers.splice(numbers.end(), additional_numbers);
    print("Ex05 after splicing: {}\n", numbers);

    numbers.remove_if([](int n) { return n % 2 == 0; });
    print("Ex05 after removing even: {}\n", numbers);
  }

  /***********************************************************************************/
  // 06_forward_list.cpp
  {
    // template <typename string, typename int>
    class ChainedHashTable {
     public:
      ChainedHashTable(size_t capacity) : capacity(capacity) { table.resize(capacity); }

      bool get(const string &key, int &value) const {
        const auto &list = table[hash(key)];
        for (const auto &bucket : list) {
          if (bucket.key == key) {
            value = bucket.value;
            return true;
          }
        }
        return false;
      }

      void put(const string &key, const int &value) {
        auto &list = table[hash(key)];
        for (auto &bucket : list) {
          if (bucket.key == key) {
            bucket.value = value;
            return;
          }
        }
        list.emplace_front(key, value);
      }

      bool remove(const string &key) {
        auto &list = table[hash(key)];
        list.remove_if([&](const Bucket &bucket) { return bucket.key == key; });
        return false;
      }

     private:
      struct Bucket {
        string key;
        int value;
        Bucket(string k, int v) : key(k), value(v) {}
      };

      std::vector<std::forward_list<Bucket>> table;
      size_t capacity;
      size_t hash(const string &key) const {
        return std::hash<string>{}(key) % capacity;
      }
    };

    ChainedHashTable hashTable(10);

    hashTable.put("apple", 10);
    hashTable.put("banana", 20);
    hashTable.put("cherry", 30);

    int value;
    if (hashTable.get("apple", value)) print("apple: {}\n", value);

    if (hashTable.get("banana", value)) print("banana: {}\n", value);

    hashTable.remove("banana");

    if (!hashTable.get("banana", value)) print("no banana\n");
  }

  /***********************************************************************************/
  // 06_string.cpp
  {
    string s = "Hello, C++ World!";

    print("Ex06: Size: {}, first char {}\n", s.size(), s[0]);

    string greet = "Hello";
    string target = "World";
    string combined = greet + ", " + target + "!";
    print("Ex06: Combined: {}", combined);

    if (s.find("C++") != string::npos) print("Ex06: found 'C++'\n");

    auto to_up = [](auto c) { return std::toupper(c); };
    auto ups = s | std::views::transform(to_up) | std::ranges::to<string>();
    print("Ex06: upcase: {}\n", ups);

    auto to_low = [](auto c) { return std::tolower(c); };
    auto lows = s | std::views::transform(to_low) | std::ranges::to<string>();
    print("Ex06: lowcase: {}\n", ups);

    std::erase(s, ' ');
    print("Ex06: erase spaces {}\n", s);

    string first = "apple";
    string second = "banana";

    if (first < second) print("Ex06: {}  comes before {}\n", first, second);

    int number = 2112;
    string numStr = std::to_string(number);
    print("Ex06: number as string {}\n", numStr);

    int convertedBack = std::stoi(numStr);
    print("Ex06: string to num {}\n", convertedBack);
  }
}