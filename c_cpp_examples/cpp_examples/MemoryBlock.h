#pragma once
#include <algorithm>
#include <iostream>

class MemoryBlock {
public:
  explicit MemoryBlock(size_t length)
      : _length(length), _data(new int[length]) {
    std::cout << "MemoryBlock of size " << _length << " created." << std::endl;
  }
  ~MemoryBlock() {
    std::cout << "In ~MemoryBlock(). length = " << _length << ".";
    if (_data != nullptr) {
      std::cout << " Deleting resource.";
      delete[] _data;
    }
  }
  MemoryBlock(const MemoryBlock &other)
      : _length(other._length), _data(new int[other._length]) {
    std::cout << "Copy constructor called" << std::endl;
    std::copy(other._data, other._data + _length, _data);
  }
  MemoryBlock &operator=(const MemoryBlock &other) {
    std::cout << "In operator=" << std::endl;
    if (this != &other) {
      delete[] _data;
      _length = other._length;
      _data = new int[_length];
      std::copy(other._data, other._data + _length, _data);
    }
    return *this;
  }
  MemoryBlock(MemoryBlock &&other) noexcept
      : _length(other._length), _data(other._data) {
    std::cout << "Move constructor called" << std::endl;
    other._length = 0;
    other._data = nullptr;
  }
  MemoryBlock operator=(MemoryBlock &&other) noexcept {
    std::cout << "In move operator" << std::endl;
    if (this != &other) {
      delete[] _data;
      _length = other._length;
      _data = other._data;
      other._length = 0;
      other._data = nullptr;
    }
    return *this;
  }
  size_t Length() const { return _length; }

private:
  size_t _length;
  int *_data;
};
