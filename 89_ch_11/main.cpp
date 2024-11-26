#include <algorithm>
#include <chrono>
#include <format>
#include <iomanip>
#include <list>
#include <print>
#include <ranges>
#include <set>
#include <vector>

using std::vector, std::print, std::format, std::string;
namespace R = std::ranges;

int main() {
  /************************************************************************************/
  // 01_allnoneany_of.cpp
  {
    vector<int> grades = {85, 90, 78, 92, 88, 76, 95, 89};

    auto gt_0 = [g = 0](auto grade) { return grade > g; };
    auto cond = R::all_of(grades, gt_0);
    print("Ex01: {}\n",
          cond ? "All students have positive grades." : "Not all grades are positive.");

    auto lt_80 = [g = 80](auto grade) { return grade < g; };
    cond = R::none_of(grades, lt_80);
    print("Ex01: {}\n", cond ? "No student has scored below passing marks."
                             : "There are students who scored below passing marks.");

    auto ge_95 = [g = 95](auto grade) { return grade >= g; };
    cond = R::any_of(grades, ge_95);
    print("Ex01: {}\n",
          cond ? "There's at least one student with an 'exceptional' grade."
               : "No student has an 'exceptional' grade.");
  }

  /************************************************************************************/
  // 02_is_sorted.cpp
  {
    vector<double> prices = {5.99, 10.49, 20.89, 25.55, 30.10};

    auto cond = R::is_sorted(prices);
    print("Ex02: {}\n", cond ? "The product prices are sorted in ascending order."
                             : "The product prices are not sorted.");

    auto lt_20 = [p = 20.0](double price) { return price < p; };

    // auto partitionPoint = R::partition(prices, lt_20);

    cond = R::is_partitioned(prices, lt_20);
    print("Ex02: {}\n",
          cond ? "Prices are partitioned with prices less than $20 first."
               : "Prices are not partitioned based on the given criteria.");

    vector<double> shuffled_prices = {25.55, 5.99, 30.10, 10.49, 20.89};

    // Using std::is_permutation to ascertain if
    // shuffled_prices is a reordering of prices
    cond = R::is_permutation(prices, shuffled_prices);
    print("Ex02: {}\n", cond ? "Sequences are permutations of each other."
                             : "Sequences are not permutations of each other.");
  }

  /************************************************************************************/
  // 03_count.cpp
  {
    vector<int> grades = {85, 90, 78, 92, 88, 76, 95, 89};

    const auto exact_count = R::count(grades, 90);
    print("Ex03: Number of students who scored exactly 90: {}\n", exact_count);

    auto g = 80;
    auto gt_80 = [g](int grade) { return grade > g; };
    const auto above_count = R::count_if(grades, gt_80);
    print("Ex03: Number of students who scored above {}: {}\n", g, above_count);
  }

  /************************************************************************************/
  // 04_find_if.cpp
  {
    struct Student {
      std::string name;
      int grade{0};

      Student(std::string n, int g) : name(n), grade(g) {}
    };

    std::list<Student> students = {
        {"Lisa", 85},   {"Corbin", 92}, {"Aaron", 87},
        {"Daniel", 92}, {"Mandy", 78},  {"Regan", 92},
    };

    auto g_92 = [](const auto &s) { return s.grade == 92; };
    auto first_92 = R::find_if(students, g_92);

    if (first_92 != students.end())
      print("Ex04: {} was the first to score 92.\n", first_92->name);

    std::list<Student> search_for = {{"", 92}};
    using csref = const Student &;
    auto g_eq = [](csref s, csref value) { return s.grade == value.grade; };
    auto last_92 = R::find_end(students, search_for, g_eq).begin();

    if (last_92 != students.end())
      print("Ex04: {} was the last to score 92.\n", last_92->name);
  }

  /************************************************************************************/
  // 05_upperlower_bound.cpp
  {
    struct Student {
      std::string name;
      int grade{0};

      bool operator<(const Student &other) const { return grade < other.grade; }
    };

    std::set<Student> students = {
        {"Amanda", 68}, {"Claire", 72}, {"Aaron", 85},   {"William", 85},
        {"April", 92},  {"Bryan", 96},  {"Chelsea", 98},
    };

    Student search_student{"", 85};

    auto cmp = [](const Student &a, const Student &b) { return a.grade < b.grade; };

    const auto lb = R::lower_bound(students, search_student, cmp);

    if (lb != students.end() && lb->grade == 85)
      print("Ex05: {} is the first student with a grade of 85.\n", lb->name);

    const auto ub = R::upper_bound(students, search_student, cmp);

    if (ub != students.end())
      print(
          "Ex05: {} is the next student after the last one "
          "with a grade of 85, with a grade of {} \n",
          ub->name, ub->grade);

    print("Ex05: {}\n", R::binary_search(students, search_student, cmp)
                            ? "There's at least one student with a grade of 85."
                            : "No student has scored an 85.");
  }

  /************************************************************************************/
  // 06_ajacent_find.cpp

  {
    using std::chrono::system_clock;

    struct SensorData {
      int temperature{0};
      system_clock::time_point timestamp;
    };

    const auto now = system_clock::now();
    using std::chrono::hours;

    std::list<SensorData> sensor_readings = {
        {72, now - hours(10)}, {73, now - hours(9)}, {75, now - hours(8)},
        {75, now - hours(7)},  {76, now - hours(6)}, {78, now - hours(5)},
        {78, now - hours(4)},  {79, now - hours(3)}, {80, now - hours(2)},
        {81, now - hours(1)},
    };

    auto sens_data = [](const auto &x) {
      return format("{}: {}\n", x.timestamp, x.temperature);
    };
    print("Ex06: sensor_readings {}\n",
          sensor_readings | R::views::transform(sens_data));

    auto it = sensor_readings.begin();

    while (it != sensor_readings.end()) {
      using sd = const SensorData &;
      auto is_eq = [](sd a, sd b) { return a.temperature == b.temperature; };
      it = std::adjacent_find(it, sensor_readings.end(), is_eq);

      if (it != sensor_readings.end()) {
        int duplicateValue = it->temperature;

        print(
            "Ex06: Found consecutive duplicate readings of value: {} "
            "taken at the following times:\n",
            duplicateValue);

        while (it != sensor_readings.end() && it->temperature == duplicateValue) {
          // const auto time = system_clock::to_time_t(it->timestamp);

          print("\t{:%Y-%m-%d %H:%M:%S}\n", it->timestamp);
          ++it;
        }
      }
    }
  }

  /************************************************************************************/
  // 07_lexicographical_compare.cpp
  {
    vector<char> seq1 = {'a', 'b', 'c'};
    vector<char> seq2 = {'a', 'b', 'd'};
    vector<char> seq3 = {'a', 'b', 'c', 'd'};

    auto cmp = R::lexicographical_compare;
    print("Ex07: {}\n", cmp(seq1, seq2) ? "seq1 < seq1" : "seq1 > seq2");
    print("Ex07: {}\n", cmp(seq1, seq3) ? "seq1 < seq3" : "seq1 not < seq3");

    // strings
    std::string str1 = "apple";
    std::string str2 = "banana";

    print("Ex07: {}\n", cmp(str1, str2) ? "str1 < str2" : "str1 not < str2");
  }

  /************************************************************************************/
  // 08_minmax_element.cpp
  {
    vector<int> scores = {85, 93, 78, 90, 96, 82};
    print("Ex08: scores {}\n", scores);

    const auto max_it = R::max_element(scores);
    print("{}", (max_it != scores.end()) ? format("Ex08: highest: {}\n", *max_it) : "");

    const auto min_it = R::min_element(scores);
    print("{}", (min_it != scores.end()) ? format("Ex08: lowest: {}\n", *min_it) : "");

    const auto [min, max] = R::minmax_element(scores);
    auto cond = (min != scores.end() && max != scores.end());
    print("{}", cond ? format("Ex08: lowest: {}, highest: {}\n", *min, *max) : "");

    // Find the minimum and maximum prices
    vector<double> product_prices = {99.99, 79.99, 49.99, 59.99, 89.99};

    auto [min_p, max_p] = R::minmax_element(product_prices);

    cond = (min_p != product_prices.end() && max_p != product_prices.end());
    constexpr auto fmt = "Ex08: lowest price: {}, highest price: {}\n";
    print("{}", cond ? format(fmt, *min_p, *max_p) : "");

  }
}