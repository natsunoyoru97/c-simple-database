// Copyright 2022 natsunoyoru97
//
// Created by natsunoyoru on 23-1-2.
//

#ifndef SRC_PAGER_PAGER_H_
#define SRC_PAGER_PAGER_H_

#include <cstdint>

constexpr uint32_t TABLE_MAX_PAGES = 1024;

namespace pager {

class Pager {
 public:
  // TODO(natsunoyoru97): considering making some of the member variables
  // private
  int fd_;
  uint32_t file_len_;
  // TODO(natsunoyoru97): use std::array instead
  void* pages_[TABLE_MAX_PAGES];

  explicit Pager(const char* filename);
  ~Pager();

  void* GetPage(uint32_t page_num);
  void Flush(uint32_t page_num, uint32_t size);
};

}  // namespace pager

#endif  // SRC_PAGER_PAGER_H_
