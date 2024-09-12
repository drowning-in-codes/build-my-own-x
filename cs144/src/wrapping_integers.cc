#include "wrapping_integers.hh"

#include <cmath>
using namespace std;

Wrap32 Wrap32::wrap(uint64_t n, Wrap32 zero_point) {
  // Your code here.
  // uint32_t raw_value =
  // (zero_point.raw_value_ + n) % (1L<<32);  // or use mod?
  return Wrap32{static_cast<uint32_t>(zero_point.raw_value_ + n)};
}

uint64_t Wrap32::unwrap(Wrap32 zero_point, uint64_t checkpoint) const {
  // Your code here.
  const uint64_t GAP = 1UL << 32;
  // absolute seqno % INT32_RANGE == offset
  uint32_t offset = raw_value_ - zero_point.raw_value_; // 如果这里右边是负数,读取这个数据会进行类似+1UL << 32操作.
  if (checkpoint > offset) {
    uint64_t real_checkpoint = checkpoint - offset + (GAP >> 1);
    uint64_t wrap_num = real_checkpoint / GAP;
    return wrap_num*GAP +offset;
  } else {
    return offset;
  }
}
