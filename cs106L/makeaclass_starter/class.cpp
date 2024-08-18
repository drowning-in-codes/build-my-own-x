#include "class.h"
int myConstucor::getX() const { return m_x; }
void myConstucor::setX(int t_x) { this->m_x = t_x; }
int myConstucor::getY() const { return m_y; }
void myConstucor::setY(int t_y) { this->m_y = t_y; }
int myConstucor::getMult() const { return m_x * m_y; }
myConstucor::myConstucor(int t_x, int t_y) : m_x(t_x), m_y(t_y) {}
std::ostream &myConstucor::operator<<(std::ostream &os) {
  os << m_x << " " << m_y;
  return os;
}
std::ostream &operator<<(std::ostream &os, const myConstucor &mc) {
  os << mc.m_x << " " << mc.m_y;
  return os;
}
