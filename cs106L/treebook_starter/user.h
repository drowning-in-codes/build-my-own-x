#include <set>
#include <string>

class User {
public:
  // TODO: write special member functions, including default constructor!

  User() {
    this->name = "";
    this->friends = {};
  }
  // 拷贝构造
  User(const User &user) {
    this->name = user.name;
    this->friends = user.friends;
  }

  // 拷贝赋值
  // User &operator=(const User &user) {
  //   if (this == &user)
  //     return *this;
  //   this->name = user.name;
  //   this->friends = user.friends;
  //   return *this;
  // }
  // 移动构造
  User(User &&user) {
    this->name = user.name;
    this->friends = user.friends;
    user.name = "";
    user.friends = {};
  }
  // 移动赋值
  User &operator=(User &&user) {
    if (this == &user)
      return *this;
    this->name = user.name;
    this->friends = user.friends;
    user.name = "";
    user.friends = {};
    return *this;
  }

  User(std::string name) {
    this->name = name;
    this->friends = {};
  }

  bool operator<(const User &user) const { return this->name < user.name; }
  // getter functions
  std::string getName() const;
  std::set<User> &getFriends();
  const std::set<User> &getFriends() const;

  // setter functions
  void setName(std::string name);
  friend void operator+(User &user1, User &user2);
  // TODO: add the < operator overload here!

private:
  std::string name;
  std::set<User> friends;
};
