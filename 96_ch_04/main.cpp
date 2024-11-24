#include <algorithm>
#include <chrono>
#include <format>
#include <iterator>
#include <mutex>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

using std::generate_n, std::back_inserter;
using std::string, std::string_view;
using std::vector, std::print;

int main() {
  // 01_customsort.cpp
  {
    struct Person {
      std::string name;
      int age{0};
      Person(std::string n, int a) : name(n), age(a) {}
    };

    std::vector<Person> people = {
        Person("Regan", 30),
        Person("Lisa", 40),
        Person("Corbin", 45),
        Person("John", 99),
    };

    // sort by name
    std::ranges::sort(people,
                      [](const Person &a, const Person &b) { return a.name < b.name; });

    auto v1 = people | std::views::transform([](Person &p) {
                return std::format("{} {}", p.name, p.age);
              });

    print("By Name: {}\n", v1);

    // sort by Age
    std::ranges::sort(people,
                      [](const Person &a, const Person &b) { return a.age < b.age; });

    auto v2 = people | std::views::transform([](Person &p) {
                return std::format("{} {}", p.name, p.age);
              });

    print("By Age: {}\n", v2);
  }

  // 02_invalidrange.cpp
  {
    vector<int> numbers = {3, 1, 4, 1, 5, 9};

    // Move iterator beyond end undefined behavior due to the invalid range
    // std::sort(numbers.begin(), numbers.end() + 1);

    print("Ex02: {}\n", numbers);

    // Comparator will return true even when both elements are equal.
    // This violates the strict weak ordering.
    auto bad_comparator = [](int a, int b) { return a <= b; };

    // Using such a comparator can lead to unexpected results.
    std::ranges::sort(numbers, bad_comparator);
    print("Ex02: {}\n", numbers);
  }

  // 03_lowerboundupperbound.cpp
  {
    auto LB = std::ranges::lower_bound;
    auto UB = std::ranges::upper_bound;

    vector<int> numbers = {1, 3, 3, 5, 7};

    int val1 = 3;
    auto low1 = LB(numbers, val1);
    print("Ex03: lower_bound for value {} : {}\n", val1, (low1 - numbers.begin()));

    int val2 = 4;
    auto low2 = LB(numbers, val2);
    print("Ex03: lower_bound for value {} : {}\n", val2, (low2 - numbers.begin()));

    int val3 = 3;
    auto up1 = UB(numbers, val3);
    print("Ex03: upper_bound for value {} : {}\n", val3, (up1 - numbers.begin()));
  }

  // 04_playlist.cpp
  {
    using VS = vector<string>;
    auto rev = std::views::reverse | std::ranges::to<vector<string>>();

    VS playlist = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L"};

    auto r_playlist = playlist | rev;
    std::ranges::rotate(r_playlist, r_playlist.begin() + 10);

    playlist = r_playlist | rev;
    print("{::}\n", playlist | std::views::join);

    // insert in the middle
    VS newAlbum = {"1", "2", "3"};
    playlist.insert(playlist.begin() + playlist.size() / 2, newAlbum.begin(),
                    newAlbum.end());
    print("{::} after new albums\n", playlist | std::views::join);

    VS springSongs = {"S1", "S2", "S3 ", "S4"};

    if (playlist.size() < springSongs.size()) {
      playlist.resize(springSongs.size());
    }

    std::ranges::fill(playlist.begin(), playlist.begin() + springSongs.size(),
                      "Spring Song");

    print("{:} spring refresh\n", playlist);
  }

  // 05_iteratorinvalidation.cpp
  {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    print("Ex05: {}\n", numbers);

    std::vector<int>::iterator it = numbers.begin() + 2;
    print("Ex05: element at the iterator: {}\n", *it);

    for (int i = 6; i <= 10; i++) {
      numbers.push_back(i);
    }

    // this is bad "it" is invalid after push_back
    // print("Ex05: element at the iterator after push: {}\n", *it);

    it = numbers.begin() + 2;
    numbers.insert(it, 99);

    it = numbers.begin() + 3;
    numbers.erase(it);

    print("Ex05: {}\n", numbers);
  }

  // 06_iteratorinvalidation2.cpp
  {
    using clock = std::chrono::high_resolution_clock;
    using std::chrono::duration;

    vector<int> numbers;
    auto N{1'000'000};
    numbers.reserve(N);

    auto st1 = clock::now();
    generate_n(back_inserter(numbers), N, [i = 0] mutable { return i++; });
    auto end1 = clock::now();
    duration<double> elapsed = end1 - st1;

    print("Ex06: {} generate_n back_inserter\n", elapsed.count());

    st1 = clock::now();
    for (int i = 1; i <= N; ++i) {
      numbers.push_back(i);
    }
    end1 = clock::now();
    elapsed = end1 - st1;
    print("Ex06: {} for push_back\n", elapsed.count());

    size_t positionOfFive = 4;  // 0-based index for number 5 in our vector
    print("Ex06: Fifth element: {} \n", numbers[positionOfFive]);

    numbers.insert(numbers.begin() + 5, 99);

    std::vector<int>::iterator it = numbers.begin() + positionOfFive;
    print("Ex06: Element at the earlier fifth position after insertion: {}\n", *it);

    // After inserting, refresh the iterator
    it = numbers.begin() + 6;

    std::sort(numbers.begin(), numbers.end());

    // Caution with Custom Comparators and Predicates:
    auto isOdd = [](int num) { return num % 2 != 0; };
    auto countOdd = std::count_if(numbers.begin(), numbers.end(), isOdd);
    print("Ex06: Number of odd values: {}\n", countOdd);

    // Note: The lambda function 'isOdd' is just a read-only
    // operation and doesn't modify the vector, ensuring we
    // don't have to worry about invalidation.
  }

  // 07_iteratorinvalidationthreaded.cpp
  {
    std::mutex vecMutex;

    auto add_to_vector = [&vecMutex](std::vector<int> &numbers, int value) {
      std::lock_guard<std::mutex> guard(vecMutex);
      numbers.push_back(value);
    };

    auto print_vector = [&vecMutex](const std::vector<int> &numbers) {
      std::lock_guard<std::mutex> guard(vecMutex);
      print("Ex07: {}", numbers);
    };

    std::vector<int> numbers;

    std::thread t1(add_to_vector, std::ref(numbers), 1);
    std::thread t2(add_to_vector, std::ref(numbers), 2);

    t1.join();
    t2.join();

    std::thread t3(print_vector, std::ref(numbers));
    t3.join();
  }
}
