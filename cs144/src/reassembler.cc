#include "reassembler.hh"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
using namespace std;

void Reassembler::insert(uint64_t first_index, string data,
                         bool is_last_substring) {
  auto pos_iter = substrings.upper_bound(first_index);
  if (pos_iter != substrings.begin()) {
    // 获得index前面的数据
    pos_iter--;
  }
  uint64_t new_idx = first_index;
  if (pos_iter != substrings.end() && pos_iter->first <= first_index) {
    const uint64_t up_idx = pos_iter->first;
    if (first_index < up_idx + pos_iter->second.length()) {
      // update first_index
      new_idx = up_idx + pos_iter->second.length();
    }
  } else if (first_index < next_assembled_idx) {
    // update first_index
    new_idx = next_assembled_idx;
  }
  const uint64_t data_start_pos = new_idx - first_index;
  // truncated size
  ssize_t data_size = data.size() - data_start_pos;
  pos_iter = substrings.lower_bound(new_idx);
  while (pos_iter != substrings.end() && new_idx <= pos_iter->first) {
    const size_t data_end_pos = new_idx + data_size;
    // 如果存在重叠区域
    if (pos_iter->first < data_end_pos) {
      // 如果是部分重叠
      if (data_end_pos < pos_iter->first + pos_iter->second.size()) {
        data_size = pos_iter->first - new_idx;
        break;
      }
      // 如果是全部重叠
      else {
        stored_bytes_num -= pos_iter->second.size();
        pos_iter = substrings.erase(pos_iter);
        continue;
      }
    }
    // 如果不重叠则直接 break
    else
      break;
  }
  uint64_t first_unacceptable_index =
      next_assembled_idx + writer().available_capacity();
  if (new_idx >= first_unacceptable_index) {
    return;
  }
  if (data_size > 0) {
    const string updated_data = data.substr(data_start_pos, data_size);
    uint64_t cap = output_.writer().available_capacity();
    uint64_t len = updated_data.length();

    if (new_idx == next_assembled_idx) {
      // 满足条件 推到byte_stream中
      output_.writer().push(updated_data);
      next_assembled_idx += min(len, cap);
    } else {
      // 存到substrings中 超出cap的truncate
      if (new_idx + data_size > first_unacceptable_index) {
        uint64_t overflow_size = new_idx + data_size - first_unacceptable_index;
        substrings[new_idx] = std::move(updated_data.substr(0, overflow_size));
        stored_bytes_num += overflow_size;
      } else {
        substrings[new_idx] = std::move(updated_data);
        stored_bytes_num += len;
      }
    }
  }
  // 处理重叠字符串
  for (auto it = substrings.begin(); it != substrings.end();) {
    assert(next_assembled_idx <= it->first);
    if (it->first == next_assembled_idx) {
      uint64_t len = it->second.length();
      output_.writer().push(it->second);
      next_assembled_idx += len;
      stored_bytes_num -= len;
      it = substrings.erase(it);
    } else {
      break;
    }
  }
  if (is_last_substring) {
    max_index = first_index + data.size();
  }
  if (max_index <= next_assembled_idx) {
    output_.writer().close();
  }
}

uint64_t Reassembler::bytes_pending() const {
  // Your code here.
  return stored_bytes_num;
}
