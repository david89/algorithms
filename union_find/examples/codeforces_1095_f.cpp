#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <functional>
#include <list>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
using namespace std;

using ll = long long;

// Example:
// https://codeforces.com/contest/1095/problem/F

// UnionFind is a not thread-safe class used to represent disjoint-set data.
// More details in https://en.wikipedia.org/wiki/Disjoint-set_data_structure
class UnionFind {
public:
  explicit UnionFind(int n) {
    nodes_.resize(n);
    for (int i = 0; i < n; i++) {
      nodes_[i] = {i, 1};
    }
  }

  // Finds the representative of the given node `x`.
  int Find(int x) {
    if (!ValidIndex(x)) throw std::out_of_range("Out of range");
    return FindImpl(x);
  }

  // Returns true after merging the subtrees rooted at x and y if they were in
  // different components.
  bool Union(int x, int y) {
    if (!ValidIndex(x) || !ValidIndex(y)) {
      throw std::out_of_range("Out of range");
    }

    int root_x = Find(x);
    int root_y = Find(y);
    if (root_x == root_y) return false;

    // Make root_x the subtree with the greatest height.
    if (nodes_[root_x].height < nodes_[root_y].height) {
      std::swap(root_x, root_y);
    }

    // Now root_x is thre representative of the root_y subtree.
    nodes_[root_y].parent = root_x;

    // If we have two subtrees with the same height, then we need to increment
    // the height of root_x.
    if (nodes_[root_x].height == nodes_[root_y].height) {
      nodes_[root_x].height = nodes_[root_y].height + 1;
    }

    return true;
  }

private:
  struct TreeNode {
    int parent;
    int height;
  };

  inline bool ValidIndex(int x) const {
    return x >= 0 && x < nodes_.size();
  }

  int FindImpl(int x) {
    if (nodes_[x].parent == x) {
      return x;
    }
    // Path compression: make every node point to its representative.
    return nodes_[x].parent = FindImpl(nodes_[x].parent);
  }

  std::vector<TreeNode> nodes_;
};

struct Edge {
  int x;
  int y;
  ll value;

  bool operator<(const Edge& other) const {
    return value < other.value;
  }
};

int main() {
  cin.sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m;
  cin >> n >> m;

  vector<ll> v(n);
  for (ll& e : v) cin >> e;

  vector<Edge> edges(m);
  for (auto& e : edges) {
    cin >> e.x >> e.y >> e.value;
    e.x--;
    e.y--;
  }

  int best = min_element(begin(v), end(v)) - begin(v);
  for (int i = 0; i < n; i++) {
    if (i == best) continue;
    edges.push_back(Edge{i, best, v[i] + v[best]});
  }

  sort(begin(edges), end(edges));

  ll ans = 0;
  UnionFind uf(n);
  for (const auto& e : edges) {
    if (uf.Union(e.x, e.y)) {
      ans += e.value;
    }
  }

  cout << ans << endl;

  return 0;
}
