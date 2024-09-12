#include <cstdio>
#include <iostream>
#include <map>
#include <vector>
using namespace std;
class Value {
public:
  int v{1};
  int v2{v * 2};
  float v3;
  bool t;
  Value() : v{2} { puts(__PRETTY_FUNCTION__); }
};

struct TCPSenderMessage {

  bool SYN{};
  std::string payload{};
  bool FIN{};

  bool RST{};

  // How many sequence numbers does this segment use?
  size_t sequence_length() const { return SYN + payload.size() + FIN; }
};
void push() {
  TCPSenderMessage sender_message{};
  sender_message.SYN = true;
  cout << sender_message.sequence_length() << endl;
}
struct Testconst {
  int value{};
  // bool operator<(const Testconst &other) const { return value < other.value;
  // }
  auto operator<=>(const Testconst &other) const {
    return value <=> other.value;
  };
  // bool operator==(const Testconst &other) const { return value ==
  // other.value; }
};
struct Person {
  std::string name{};
  double value{};
  constexpr auto operator<=>(const Person &) const = default;
};
int main() {
  Person p1{.name = "张三", .value = 12.}, p2{.name = "李四", .value = 13.};
  if (p1 == p2) {
    puts("我们都一样");
  }
  std::map<Testconst, int> test_map{};
  test_map.insert({Testconst{1}, 1});
  test_map[Testconst{1}] = 2;
  test_map.insert({Testconst{2}, 2});
  for (const auto &[key, value] : test_map) {
    cout << key.value << "|" << value << endl;
  }

  TCPSenderMessage sender_message{};
  sender_message.SYN = true;
  cout << sender_message.sequence_length() << endl;
  std::map<uint64_t, TCPSenderMessage> queue{};
  queue[1] = sender_message;
  Value value;
  std::vector<int> vec{1};
  auto it = vec.begin();
  if (it == vec.end()) {
    cout << "fuck";
  }
  cout << value.v << "|" << value.v2 << "|" << value.v3 << endl;
  cout << value.t << endl;
  return 0;
}
