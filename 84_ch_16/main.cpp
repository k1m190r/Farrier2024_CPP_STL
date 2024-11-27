#include <algorithm>
#include <print>
#include <ranges>
#include <unordered_map>
#include <vector>

using std::vector, std::print, std::string;
namespace R = std::ranges;
namespace V = std::ranges::views;

int main() {
  /************************************************************************************/
  // 01_errorhandling.cpp
  {
    class CType {
     public:
      CType(int v = 0) : value(v) {}
      bool operator<(const CType &o) const { return value < o.value; }
      CType operator+(const CType &o) const { return CType(value + o.value); }
      int get() { return value; }

     private:
      int value{0};
    };

    class CContainer {
     public:
      using iter = vector<CType>::iterator;
      using const_iter = vector<CType>::const_iterator;
      iter begin() { return data.begin(); }
      const_iter begin() const { return data.begin(); }
      iter end() { return data.end(); }
      const_iter end() const { return data.end(); }
      void push_back(const CType &v) { data.push_back(v); }

     private:
      std::vector<CType> data;
    };

    CContainer cont;
    cont.push_back(CType(3));
    cont.push_back(CType(1));
    cont.push_back(CType(2));
    cont.push_back(CType(99));

    try {
      std::sort(cont.begin(), cont.end());
      auto get = [](auto x) { return x.get(); };
      auto p = R::subrange(cont.begin(), cont.end()) | V::transform(get);

      print("{}\n", p);
    } catch (const std::exception &e) {
      print(stderr, "An error occurred: {}\n", e.what());
      return 1;
    }
  }

  /************************************************************************************/
  // 02_exceptionsafety.cpp
  {
    class CType {
     public:
      using iter = vector<int>::iterator;
      using const_iter = vector<int>::const_iterator;
      CType() = default;
      CType(const CType &o) : data(o.data) {}
      CType(CType &&o) noexcept : data(std::move(o.data)) {}
      CType &operator=(const CType &o) {
        if (this != &o) {
          data = o.data;
        }
        return *this;
      }
      CType &operator=(CType &&o) noexcept {
        if (this != &o) {
          data = std::move(o.data);
        }
        return *this;
      }
      ~CType() = default;
      size_t size() const { return data.size(); }
      bool empty() const { return data.empty(); }
      int &operator[](size_t i) { return data[i]; }
      const int &operator[](size_t i) const { return data[i]; }
      void push_back(int v) { data.push_back(v); }
      void pop_back() { data.pop_back(); }
      iter begin() { return data.begin(); }
      const_iter begin() const { return data.begin(); }
      iter end() { return data.end(); }
      const_iter end() const { return data.end(); }

     private:
      vector<int> data;
    };

    CType custom;

    for (int i = 0; i < 10; ++i) {
      custom.push_back(i);
    }

    R::for_each(custom, [](int &value) { print("{} ", value); });
    print("\n");
  }

  /************************************************************************************/
  // 03_customiterator.cpp
  {
    class MContain {
     public:
      MContain(std::initializer_list<int> v) : data(v) {}
      class itr {
       private:
        vector<int>::iterator it;

       public:
        itr(vector<int>::iterator iter) : it(iter) {}
        int &operator*() { return *it; }
        itr &operator++() {
          ++it;
          return *this;
        }
        bool operator==(const itr &o) const { return it == o.it; }
        bool operator!=(const itr &o) const { return it != o.it; }
      };
      itr begin() { return itr(data.begin()); }
      itr end() { return itr(data.end()); }

     private:
      vector<int> data;
    };

    MContain container = {1, 2, 3, 4, 5};

    for (MContain::itr it = container.begin(); it != container.end(); ++it) {
      print("{} ", *it);
    }
    print("\n");
  }

  /************************************************************************************/
  // 04_operatoroverloading.cpp
  {
    class Product {
     public:
      string name;
      double price;
      Product(const string &n, double p) : name(n), price(p) {}
      Product operator+(const Product &o) const {
        return Product(name + " and " + o.name, price + o.price);
      }
      bool operator<(const Product &o) const { return price < o.price; }
      Product &operator=(const Product &o) {
        if (this == &o) {
          return *this;
        }
        name = o.name;
        price = o.price;
        return *this;
      }
      Product &operator+=(const Product &o) {
        price += o.price;
        return *this;
      }
    };

    Product widget("Widget", 25.99);
    Product gadget("Gadget", 19.95);

    Product combinedProduct = widget + gadget;

    widget += gadget;

    bool widgetIsCheaper = widget < gadget;
    bool gadgetIsCheaper = gadget < widget;

    print("Combined Product: {} {}\n", combinedProduct.name, combinedProduct.price);
    print("Is Widget cheaper than Gadget? {}\n", (widgetIsCheaper ? "Yes" : "No"));
    print("Is Gadget cheaper than Widget? {}\n", (gadgetIsCheaper ? "Yes" : "No"));
    print("Updated widget: {} {}\n", widget.name, widget.price);
  }

  /************************************************************************************/
  // 05_customhash.cpp
  {
    class Person {
     public:
      Person(const string &n, int a) : name(n), age(a) {}
      string getName() const { return name; }
      int getAge() const { return age; }
      bool operator==(const Person &o) const { return name == o.name && age == o.age; }

     private:
      string name;
      int age{0};
    };

    struct PersonHash {
      std::size_t operator()(const Person &p) const {
        std::size_t nameHash = std::hash<string>()(p.getName());
        std::size_t ageHash = std::hash<int>()(p.getAge());
        return nameHash ^ ageHash;
      }
    };

    std::unordered_map<Person, string, PersonHash> persons_map;

    Person person_1("Alice", 30);
    Person person_2("Bob", 25);

    persons_map[person_1] = "Engineer";
    persons_map[person_2] = "Designer";

    print("Alice's profession: {}\n", persons_map[person_1]);
  }
}