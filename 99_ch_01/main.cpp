#include <fmt/ranges.h>
#include <ranges>
#include <string_view>
#include <vector>

using std::string, std::string_view;
using std::vector, fmt::print, fmt::format;
namespace views = std::views;

// 05_initializingvector.cpp
class Person {
public:
  Person() = default;
  Person(string_view n, int a) : name(n), age(a) {}
  Person(string fn, string ln) : name(fn + " " + ln) {}

  void display() const { print("  Name: {}, Age: {}\n", name, age); }

  // private:
  string name;
  int age{0};
};

// MAIN

int main() {

  // 01_cstylearray.cpp
  {
    int *cArray = new int[5];

    for (int i = 0; i < 5; ++i) {
      cArray[i] = i + 1;
    }

    vector<int> v(cArray, cArray + 5);
    print("Ex01: v: {}\n", v);

    const int newSize = 7;
    int *newCArray = new int[newSize];

    for (int i = 0; i < 5; ++i) {
      newCArray[i] = cArray[i];
    }

    delete[] cArray;

    cArray = newCArray;

    vector<int> v2(newCArray, newCArray + newSize);
    print("Ex01: v2: {}\n", v2);

    int arraySize = newSize;
    print("Ex01: Size of cArray: {}\n", arraySize);

    delete[] cArray;
  }

  // 02_stdvector.cpp
  {
    std::vector<int> stlVector = {1, 2, 3, 4, 5};
    print("Ex02: stlVector {}\n", stlVector);

    stlVector.resize(7);
    print("Ex02: stlVector resized {} len {}\n", stlVector, stlVector.size());
  }

  // 03_cstylearray.cpp
  {
    int *cArray = new int[5];

    for (int i = 0; i < 5; ++i) {
      cArray[i] = i + 1;
    }

    int *temp = new int[10];
    for (int i = 0; i < 5; ++i) {
      temp[i] = cArray[i];
    }
    delete[] cArray; // Important: free the old memory
    cArray = temp;

    for (int i = 5; i < 10; ++i) {
      cArray[i] = i + 1;
    }

    vector<int> v(cArray, cArray + 10);
    print("Ex03: {}\n", v);

    delete[] cArray;
  }

  // 04_stdvector.cpp
  {
    vector<int> myVector(5);

    for (int i = 0; i < 5; ++i) {
      myVector[i] = i + 1;
    }

    for (int i = 5; i < 12; ++i) {
      myVector.push_back(i + 1);
    }

    print("Ex04: {}\n", myVector);
  }

  // 05_initializingvector.cpp
  {
    print("Ex05:\n");
    vector<Person> people;
    people.push_back(Person("Lisa", 30));
    people.push_back(Person("Corbin", 25));
    people.emplace_back("John", 33);

    people.resize(5);
    people[3] = Person("Aaron", 28);

    auto tx = [](auto &p) { return format("{} @ {}", p.name, p.age); };

    print("{}\n", people | views::transform(tx));
  }

  // 06_randomaccess.cpp
  {
    vector<int> numbers = {10, 20, 30, 40, 50};

    const auto secondElement = numbers[1];
    print("Ex06:  2nd {}\n", secondElement);

    // Undefined behavior!
    numbers.reserve(20);
    const auto outOfBoundsElement = numbers[10];
    print("Ex06: 10th {} out of bounds\n", outOfBoundsElement);
  }

  // 07_outofrange.cpp
  {
    vector<int> numbers = {10, 20, 30, 40, 50};

    try {
      const auto secondElement = numbers.at(1);
      print("Ex07:  2nd {}\n", secondElement);
    } catch (const std::out_of_range &e) {
      print("Ex07:  Error {}\n", e.what());
    }

    numbers.reserve(20);
    try {
      const auto outOfBoundsElement = numbers.at(10);
      print("Ex07: 10th {} out of bounds\n", outOfBoundsElement);
    } catch (const std::out_of_range &e) {
      print("Ex07:  Error {}\n", e.what());
    }
  }

  // 08_firstandlast.cpp
  {
    vector<int> numbers = {11, 20, 30, 40, 55};

    const auto firstElement = numbers.front();
    print("Ex08: first {}\n", firstElement);

    const auto lastElement = numbers.back();
    print("Ex08: last {}\n", lastElement);
  }

  // 09_vectorsize.cpp
  {
    vector<int> data = {1, 2, 3, 4, 5};

    const auto elementCount = data.size();
    print("Ex09: elementCount {}\n", elementCount);
  }

  // 10_pushback.cpp
  {
    vector<int> scores;

    print("Ex10: initial scores {}\n", scores);

    scores.push_back(95);

    print("Ex10: size {}\n", scores.size());
    print("Ex10: after push {}\n", scores);
  }

  // 11_insert.cpp
  {
    vector<int> scores = {90, 92, 97};

    print("Ex11: init {}\n", scores);
    scores.push_back(95);
    print("Ex11: after push {}\n", scores);
    scores.insert(scores.begin() + 2, 85);
    print("Ex11: after insert {}\n", scores);
  }

  // 12_emplace.cpp
  {
    std::vector<Person> people;
    people.emplace_back("John", "Doe");
    people.emplace(people.begin(), "Jane", "Doe");

    print("Ex12: {}\n",
          people | views::transform([](auto &p) { return p.name; }));
  }

  // 13_capacity.cpp
  {
    vector<int> numbers;

    print("Ex13: init {} size {} cap {}\n", numbers, numbers.size(),
          numbers.capacity());

    for (auto i = 1; i <= 10; ++i) {
      numbers.push_back(i);
    }

    print("Ex13: after push 10 {} size {} cap {}\n", numbers, numbers.size(),
          numbers.capacity());

    for (auto i = 11; i <= 20; ++i) {
      numbers.push_back(i);
    }
    print("Ex13: after more 20 {} size {} cap {}\n", numbers, numbers.size(),
          numbers.capacity());

    for (auto i = 0; i < 5; ++i) {
      numbers.pop_back();
    }

    print("Ex13: after remove 5 {} size {} cap {}\n", numbers, numbers.size(),
          numbers.capacity());
  }

  // 14_clear.cpp
  {
    vector<int> numbers = {1, 2, 3, 4, 5};
    print("Ex14: after clear(): {}, size: {}, cap: {}\n", numbers,
          numbers.size(), numbers.capacity());
    numbers.clear();
    print("Ex14: after clear(): {}, size: {}, cap: {}\n", numbers,
          numbers.size(), numbers.capacity());
  }
}
