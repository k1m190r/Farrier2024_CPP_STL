#include <algorithm>
#include <iostream>
#include <iterator>
#include <print>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::print, std::string, std::vector;

int main() {
  /************************************************************************************/
  // 01_unordered_set.cpp
  {
    auto display_set = [](auto x) { print("Ex01: {}\n", x); };

    std::unordered_set<int> numbers;

    std::generate_n(std::inserter(numbers, numbers.end()), 10,
                    [i = 0] mutable { return i++; });

    display_set(numbers);

    int search_5 = 5;
    if (numbers.find(search_5) != numbers.end())
      print("Ex01: {} found in the set.\n", search_5);
    else
      print("Ex01: {} NOT found in the set.\n", search_5);

    numbers.erase(5);
    display_set(numbers);

    print("Ex01: Size: {}\n", numbers.size());
    print("Ex01: Load factor: {}\n", numbers.load_factor());
    print("Ex01: buckets: {}\n", numbers.bucket_count());

    numbers.rehash(50);
    print("Ex01: buckets after rehash: {}\n", numbers.bucket_count());

    vector<int> more_numbers = {100, 101, 102, 103};
    numbers.insert_range(more_numbers);
    display_set(numbers);
  }

  /************************************************************************************/
  // 02_unordered_map.cpp
  {
    auto disp_map = [](auto x) { print("Ex02: {}\n", x); };

    std::unordered_map<std::string, int> ageMap;

    ageMap["Lisa"] = 28;
    ageMap["Corbin"] = 25;
    ageMap["Aaron"] = 30;

    std::cout << "Corbin's age: " << ageMap["Corbin"] << '\n';

    if (ageMap.find("Daisy") == ageMap.end()) {
      std::cout << "Daisy not found in the map." << '\n';
    } else {
      std::cout << "Daisy's age: " << ageMap["Daisy"] << '\n';
    }

    ageMap["Lisa"] = 29;
    std::cout << "Lisa's updated age: " << ageMap["Lisa"] << '\n';

    disp_map(ageMap);

    std::cout << "Load factor: " << ageMap.load_factor() << '\n';
    std::cout << "Bucket count: " << ageMap.bucket_count() << '\n';

    ageMap.rehash(50);
    std::cout << "Bucket count after rehash: " << ageMap.bucket_count() << '\n';

    // Remove an entry
    ageMap.erase("Aaron");
    disp_map(ageMap);
  }

  /************************************************************************************/
  // 03_03_unordered_multiset.cpp
  {
    std::unordered_multiset<std::string> fruits;

    fruits.insert("apple");
    fruits.insert("banana");
    fruits.insert("apple");
    fruits.insert("orange");
    fruits.insert("apple");
    fruits.insert("mango");
    fruits.insert("banana");

    const auto appleCount = fruits.count("apple");
    std::cout << "Number of apples: " << appleCount << '\n';

    auto found = fruits.find("orange");
    if (found != fruits.end()) {
      std::cout << "Found: " << *found << '\n';
    } else {
      std::cout << "Orange not found!" << '\n';
    }

    auto range = fruits.equal_range("banana");
    for (auto itr = range.first; itr != range.second; ++itr) {
      std::cout << *itr << " ";
    }
    std::cout << '\n';

    fruits.erase("apple");
    std::cout << "Number of apples after erase: " << fruits.count("apple") << '\n';
    std::cout << "Load factor: " << fruits.load_factor() << '\n';
    std::cout << "Bucket count: " << fruits.bucket_count() << '\n';

    fruits.rehash(50);
    std::cout << "Bucket count after rehashing: " << fruits.bucket_count() << '\n';

    for (const auto &fruit : fruits) {
      std::cout << fruit << " ";
    }
    std::cout << '\n';
  }

  /************************************************************************************/
  // 04_unordered_multimap.cpp
  {
    std::unordered_multimap<std::string, int> grades;

    grades.insert({"Lisa", 85});
    grades.insert({"Corbin", 92});
    grades.insert({"Lisa", 89});
    grades.insert({"Aaron", 76});
    grades.insert({"Corbin", 88});
    grades.insert({"Regan", 91});

    size_t lisaCount = grades.count("Lisa");
    std::cout << "Number of grade entries for Lisa: " << lisaCount << '\n';

    auto range = grades.equal_range("Lisa");
    for (auto it = range.first; it != range.second; ++it) {
      std::cout << it->first << " has grade: " << it->second << '\n';
    }

    auto lisaGrade = grades.find("Lisa");
    if (lisaGrade != grades.end()) {
      lisaGrade->second = 90;  // Updating the grade
    }

    grades.erase("Corbin");  // This will erase all grade
                             // entries for Corbin
    std::cout << "Number of grade entries for Corbin after erase: "
              << grades.count("Corbin") << '\n';

    std::cout << "Load factor: " << grades.load_factor() << '\n';
    std::cout << "Bucket count: " << grades.bucket_count() << '\n';

    grades.rehash(50);
    std::cout << "Bucket count after rehashing: " << grades.bucket_count() << '\n';

    for (const auto &entry : grades) {
      std::cout << entry.first << " received grade: " << entry.second << '\n';
    }
  }
}