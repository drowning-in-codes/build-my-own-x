#include "user.h"
#include <iostream>
#include <set>
#include <string>

// User::User(std::string name) {
// // TODO: Implement the additional constructor here!
//
// }
//
std::string User::getName() const { return name; }

std::set<User> &User::getFriends() { return friends; }

// A const version is needed to allow read-only access to the friends set!
const std::set<User> &User::getFriends() const { return friends; }

void User::setName(std::string name) { this->name = name; }

// TODO: Implement the < operator overload here!
void operator+(User &user1, User &user2) {
  user1.friends.insert(user2);
  user2.friends.insert(user1);
}
