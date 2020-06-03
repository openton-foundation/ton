#pragma once

#include "td/utils/Slice.h"
#include "td/utils/logging.h"

namespace td {
struct Bitset {
 public:
  td::Slice as_slice() const {
    return td::Slice(bits_).substr(0, (bits_size_ + 7) / 8);
  }
  bool get(size_t offset) const {
    auto i = offset / 8;
    if (i >= bits_.size()) {
      return false;
    }
    auto bit_i = offset % 8;
    return (bits_[i] & (1 << bit_i)) != 0;
  }

  void reserve(size_t offset) {
    auto i = offset / 8;
    if (i >= bits_.size()) {
      bits_.resize(i + 1);
    }
  }

  bool set_one(size_t offset) {
    auto i = offset / 8;
    auto bit_i = offset % 8;
    bits_size_ = std::max(bits_size_, offset + 1);
    if (i >= bits_.size()) {
      bits_.resize(std::max(i + 1, bits_.size() * 2));
    }
    auto mask = 1 << bit_i;
    if ((bits_[i] & mask) == 0) {
      bits_[i] |= mask;
      count_++;
      return true;
    }
    return false;
  }

  size_t ones_count() const {
    return count_;
  }

  void set_raw(std::string bits) {
    bits_ = std::move(bits);
    bits_size_ = 0;
    count_ = 0;
    for (size_t n = size(), i = 0; i < n; i++) {
      if (get(i)) {
        count_++;
        bits_size_ = i + 1;
      }
    }
  }

  size_t size() const {
    return bits_.size() * 8;
  }

 private:
  std::string bits_;
  size_t bits_size_{0};
  size_t count_{0};
};
}  // namespace td
