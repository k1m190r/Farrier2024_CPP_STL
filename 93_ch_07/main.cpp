#include <algorithm>
#include <map>
#include <print>
#include <ranges>
#include <set>
#include <string>
#include <vector>

using std::print, std::string, std::vector;
namespace ranges = std::ranges;

int main() {
  /************************************************************************************/
  // 01_set.cpp
  {
    std::set<int> numbers = {5, 3, 8, 1, 4};

    auto [position, was_inserted] = numbers.insert(6);
    if (was_inserted) print("Ex01: 6 was inserted into the set.\n");

    auto result = numbers.insert(5);
    if (!result.second) print("Ex01: 5 is already in the set.\n");

    if (numbers.find(3) != numbers.end()) print("Ex01: 3 is in the set.\n");

    numbers.erase(1);

    print("Ex01: Elements in the set: {}\n", numbers);

    std::set<int> more_numbers = {9, 7, 2};
    numbers.merge(more_numbers);

    print("Ex01: After merging: {}\n", numbers);

    if (numbers.count(2)) print("Ex01: 2 exists in the set.\n");

    using csref = const string &;
    using fn_type = bool (*)(csref, csref);
    auto fn = [](csref lhs, csref rhs) {
      auto cmp = [](char a, char b) { return std::tolower(a) < std::tolower(b); };
      return ranges::lexicographical_compare(lhs, rhs, cmp);
    };
    std::set<string, fn_type> case_insensitive_set{fn};

    case_insensitive_set.insert("Hello");
    if (!case_insensitive_set.insert("hello").second)
      print("Ex01: Duplicate insertion (case-insensitive) detected.\n");
  }

  /************************************************************************************/
  // 02_map.cpp
  {
    std::map<string, int> age_map = {{"Lisa", 25}, {"Corbin", 30}, {"Aaron", 22}};

    age_map["Kristan"] = 28;
    age_map.insert_or_assign("Lisa", 26);

    if (age_map.find("Corbin") != age_map.end())
      print("Ex02: Corbin exists in the map.\n");

    age_map["Aaron"] += 1;
    print("Ex02: Age records: {}\n", age_map);

    age_map.erase("Corbin");

    if (age_map.count("Regan") == 0) print("Ex02: Regan does not exist in the map.\n");

    using csref = const string &;
    using fn_type = bool (*)(csref, csref);
    // reverse lexicographic order
    auto cmp = [](csref lhs, csref rhs) { return lhs > rhs; };
    std::map<string, int, fn_type> customOrderMap{cmp};

    customOrderMap["Lisa"] = 25;
    customOrderMap["Corbin"] = 30;
    customOrderMap["Aaron"] = 22;

    print("Ex02: Custom ordered map: {}\n", customOrderMap);
  }

  /************************************************************************************/
  // 03_multiset.cpp
  {
    std::multiset<int> numbers = {5, 3, 8, 5, 3, 9, 4};

    numbers.insert(6);
    numbers.insert(5);

    print("Ex03: {}\n", numbers);
    print("Ex03: Number of 5s: {}\n", numbers.count(5));

    auto [begin, end] = numbers.equal_range(5);
    print("Ex03: {}\n", ranges::subrange(begin, end));

    numbers.erase(5);

    using ms = std::multiset<string, std::greater<>>;
    ms words = {"apple", "banana", "cherry", "apple"};
    print("Ex03: words {}\n", words);

    std::multiset<int> dataset = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    const auto start_4 = dataset.lower_bound(4);
    const auto stop_7 = dataset.upper_bound(7);

    print("Ex03: {}\n", ranges::subrange(start_4, stop_7));
  }

  /************************************************************************************/
  // 04_multimap.cpp
  {
    std::multimap<string, int> grades;

    grades.insert({"John", 85});
    grades.insert({"Corbin", 78});
    grades.insert({"Regan", 92});
    grades.insert({"John", 90});

    print("Ex04: {}\n", grades);

    print("Ex04: John's grade count: {}\n", grades.count("John"));

    auto [begin, end] = grades.equal_range("John");
    print("Ex04: John: {}\n", ranges::subrange(begin, end));

    grades.erase("John");

    print("Ex04: erase John: {}\n", grades);

    std::multimap<string, int, std::greater<>> reverseGrades = {
        {"Mandy", 82}, {"Aaron", 90}, {"Dan", 76}, {"Mandy", 87}};

    print("Ex04: revesed {}\n", reverseGrades);
  }
}