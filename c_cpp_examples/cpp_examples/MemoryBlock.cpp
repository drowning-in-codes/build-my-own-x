#include "MemoryBlock.h"
#include <vector>
int main() {
  MemoryBlock block(25);
  std::vector<MemoryBlock> v;
  v.push_back(block);
  v.push_back(std::move(block));
}
