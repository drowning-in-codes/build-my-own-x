#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream(uint64_t capacity) : capacity_(capacity) {}

bool Writer::is_closed() const {
  // Your code here.
  return _is_closed;
}

void Writer::push(string data) {
  // std::size_t data_len = data.length();
  // std::size_t avai_cap = this->available_capacity();
  // if (data_len >= avai_cap) {
  //   _buffer.insert(_buffer.end(), data.begin(), data.begin() + avai_cap);
  //   push_counts += avai_cap;
  // } else {
  //   _buffer.insert(_buffer.end(), data.begin(), data.end());
  //   push_counts += data_len;
  // }
  if (available_capacity() == 0 || data.empty() || is_closed()) {
    return;
  }
  if (data.size() > available_capacity()) {
    data = data.substr(0, available_capacity());
  }
  buffer_counts += data.length();
  push_counts += data.length();
  _buffer.emplace(std::move(data));
}

void Writer::close() {
  // Your code here.
  _is_closed = true;
}

uint64_t Writer::available_capacity() const {
  // Your code here.
  return capacity_ - buffer_counts;
}

uint64_t Writer::bytes_pushed() const {
  // Your code here.
  return push_counts;
}

bool Reader::is_finished() const {
  // Your code here.
  return _is_closed && buffer_counts == 0;
}

uint64_t Reader::bytes_popped() const {
  // Your code here.
  return pop_counts;
}

string_view Reader::peek() const {
  // Your code here.
  return _buffer.empty() ? string_view{}  // std::string_view dependents on the
                                          // initializer through its lifetime.
                         : string_view{_buffer.front()}.substr(removed_prefix);
}

void Reader::pop(uint64_t len) {
  buffer_counts -= len;
  pop_counts += len;

  while (len != 0) {
    const size_t size{_buffer.front().size() - removed_prefix};
    if (len < size) {
      removed_prefix += len;
      break;
    }
    _buffer.pop();
    removed_prefix = 0;
    len -= size;
  }
}

uint64_t Reader::bytes_buffered() const {
  // Your code here.
  return buffer_counts;
}
