#include <algorithm>
#include <print>
#include <ranges>
#include <vector>

using std::vector, std::print;
namespace R = std::ranges;
namespace V = std::ranges::views;

int main() {
  /************************************************************************************/
  // 01_badmovesemantics.cpp
  {
    vector<int> source = {10, 20, 30, 40, 50};
    vector<int> destination = std::move(source);

    print("Trying to access the 'source' vector after moving its data:\n");

    print("Ex01: Source: {}\n", source);
    print("Ex01: Destination {}\n", destination);

    source.push_back(60);
    print("Ex01: After trying to add data to 'source' post-move:\n");
    print("Ex01: Source {}\n", source);
  }

  /************************************************************************************/
  // 02_rvo.cpp
  {
    class Sample {
     public:
      Sample() { print("Ex02: Constructor called.\n"); }

      Sample(const Sample &) { print("Ex02: Copy Constructor called.\n"); }

      Sample(Sample &&) noexcept { print("Ex02: Move Constructor called.\n"); }

      ~Sample() { print("Ex02: Destructor called.\n"); }
    };

    auto createSample = []() { return Sample(); };

    print("Creating object via function return:\n");
    // expecting copy or move but none due to RVO
    Sample obj = createSample();
  }

  /************************************************************************************/
  // 03_replace.cpp
  {
    std::vector<int> values = {10, -1, 20, -2, 30};

    R::replace(values, -1, 0);  // Replace -1 with 0

    auto is_0 = [](int value) { return value < 0; };
    R::replace_if(values, is_0, 0);

    print("Ex03: values: {}\n", values);
  }
}