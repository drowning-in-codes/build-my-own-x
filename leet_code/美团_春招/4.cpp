#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <vector>

using namespace std;

struct UF {
  // 题给范围n太大, 用map存储
  map<int, int> pa;

  int find(int a) {
    // 要么返回自身要么返回类似一个链表一直指向的朋友
    if (pa[a] != a) {
      // 有朋友了 如果朋友有新的朋友,更新朋友
      pa[a] = find(pa[a]);
    }
    return pa[a];
  }
  // 0 1 2 3
  // 1 2 2 3
  bool isConnect(int a, int b) { return find(a) == find(b); }

  void add(int x, int y) { pa[find(x)] = find(y); }
};

int main() {
  int n, m, q;
  cin >> n >> m >> q;

  // 存储关系
  UF uf;
  set<pair<int, int>> relations;
  while (m--) {
    int a, b;
    cin >> a >> b;

    // 初始化
    uf.pa[a] = a;
    uf.pa[b] = b;
    // 使用set和强制规定a < b, 避免重边
    if (a > b)
      swap(a, b);
    relations.insert({a, b});
  }

  // 存储事件并维护关系
  vector<vector<int>> acts;
  while (q--) {
    int op, a, b;
    cin >> op >> a >> b;

    // 初始化
    uf.pa[a] = a;
    uf.pa[b] = b;

    if (a > b)
      swap(a, b);
    // 删除操作, 合法就删除, 不合法就跳过
    if (op == 1) {
      if (relations.find({a, b}) != relations.end())
        relations.erase({a, b});
      else
        continue;
    }

    vector<int> tmp = {op, a, b};
    acts.emplace_back(tmp);
  }

  // 用剩余的关系建立并查集
  for (auto &[a, b] : relations)
    uf.add(a, b);

  // 逆向遍历事件
  reverse(acts.begin(), acts.end());
  stack<string> ans;
  for (auto &act : acts) {
    int op = act[0], a = act[1], b = act[2];
    if (op == 1)
      uf.add(a, b);
    else {
      if (uf.isConnect(a, b))
        ans.push("Yes");
      else
        ans.push("No");
    }
  }

  // 输出答案
  while (!ans.empty()) {
    cout << ans.top() << '\n';
    ans.pop();
  }
}
