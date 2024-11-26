#include <list>
#include <print>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

using std::print, std::string, std::vector;

int main() {
  /************************************************************************************/
  // 01_stack.cpp
  {
    auto RPN_eval = [](const auto &expr) {
      std::stack<double> s;
      std::istringstream iss(expr);
      string token;

      while (iss >> token) {
        if (token == "+" || token == "-" || token == "*" || token == "/") {
          if (s.size() < 2) throw std::runtime_error("Invalid RPN expression");

          double b = s.top();
          s.pop();
          double a = s.top();
          s.pop();

          if (token == "+")
            s.push(a + b);
          else if (token == "-")
            s.push(a - b);
          else if (token == "*")
            s.push(a * b);
          else if (token == "/") {
            if (b == 0.0) {
              throw std::runtime_error("Division by zero");
            }
            s.push(a / b);
          }
        } else {
          s.push(std::stod(token));
        }
      }

      if (s.size() != 1) throw std::runtime_error("Invalid RPN expression");

      return s.top();
    };

    auto exprs = {
        "46 2 +",             // 48
        "5 1 2 + 4 * + 3 -",  // -14
        "3 4 5 * - ",         // 17
        // causes AddressSanitizer: alloc-dealloc-mismatch (operator new vs free) 
        // throw std::runtime_error("Division by zero");
        "3 0 /",              // divide by 0
    };

    try {
      for (auto ex : exprs) print("Ex01: {} = {}\n", ex, RPN_eval(ex));

    } catch (const std::exception &e) {
      print(stderr, "Error: {}\n", e.what());
    }
  }

  /************************************************************************************/
  // 02_queue.cpp
  {
    class Graph {
     public:
      Graph(int vertices) : n_vert{vertices} { adjoint_list.resize(vertices); }

      void add_edge(int v, int w) {
        adjoint_list[v].push_back(w);
        adjoint_list[w].push_back(v);
      }

      void BFS(int start_vertex) {
        vector<bool> visited(n_vert, false);
        std::queue<int> q;

        visited[start_vertex] = true;
        q.push(start_vertex);

        while (!q.empty()) {
          int curr_vert = q.front();
          print("{} ", curr_vert);
          q.pop();

          for (int neighbor : adjoint_list[curr_vert]) {
            if (!visited[neighbor]) {
              visited[neighbor] = true;
              q.push(neighbor);
            }
          }
        }
      }

     private:
      int n_vert{0};
      vector<vector<int>> adjoint_list;
    };

    Graph g(6);

    g.add_edge(0, 1);
    g.add_edge(0, 2);
    g.add_edge(1, 3);
    g.add_edge(1, 4);
    g.add_edge(2, 4);
    g.add_edge(3, 4);
    g.add_edge(3, 5);

    print("Ex02: BFS starting from vertex 0: ");
    g.BFS(0);  // Output: 0 1 2 3 4 5
    print("\n");
  }

  /************************************************************************************/
  // 03_priority_queue.cpp
  {
    using int_pair = std::pair<int, int>;
    class WeightedGraph {
     public:
      WeightedGraph(int vertices) : n_vert(vertices) { adjecent_list.resize(vertices); }

      void add_edge(int u, int v, int weight) {
        adjecent_list[u].push_back({v, weight});
        adjecent_list[v].push_back({u, weight});
      }

      void dijkstra(int start_vert) {
        std::priority_queue<int_pair, vector<int_pair>, std::greater<int_pair>> pq;
        vector<int> distances(n_vert, INT_MAX);

        pq.push({0, start_vert});
        distances[start_vert] = 0;

        while (!pq.empty()) {
          int curr_vert = pq.top().second;
          pq.pop();

          for (auto &neighbor : adjecent_list[curr_vert]) {
            int vertex = neighbor.first;
            int weight = neighbor.second;

            if (distances[vertex] > distances[curr_vert] + weight) {
              distances[vertex] = distances[curr_vert] + weight;
              pq.push({distances[vertex], vertex});
            }
          }
        }

        print("Ex03: Distances from vertex {}\n", start_vert);
        for (int i = 0; i < n_vert; ++i) {
          print("Ex03: {} -> {}\n", i, distances[i]);
        }
      }

     private:
      int n_vert{0};
      vector<std::list<int_pair>> adjecent_list;
    };

    WeightedGraph g(5);

    g.add_edge(0, 1, 9);
    g.add_edge(0, 2, 6);
    g.add_edge(0, 3, 5);
    g.add_edge(1, 3, 2);
    g.add_edge(2, 4, 1);
    g.add_edge(3, 4, 2);

    g.dijkstra(0);
  }
}
