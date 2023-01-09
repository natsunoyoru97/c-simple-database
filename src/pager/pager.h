// Copyright 2022 natsunoyoru97
//
// Created by natsunoyoru on 23-1-2.
//

#ifndef SRC_PAGER_PAGER_H_
#define SRC_PAGER_PAGER_H_

#include <array>
#include <cstdint>

#include "../util/util.h"

// Maximum pages a table stores
constexpr uint32_t TABLE_MAX_PAGES = 1024;

namespace storage {
  // TODO(natsunopyoru97): add a handler for file operation

// The indirection layer to handle file descriptor and give a block of memory
class Pager {
 private:
  int fd_;
  uint32_t file_len_;
  // Rows this block of memory has
  // NOTE(natsunoyoru97): I would like to remain this member variable for a
  // while to see if it is in need for future
  uint32_t num_rows_;
  // TODO(natsunoyoru97): how about using other data structures?
  std::array<char*, TABLE_MAX_PAGES> pages_;

 public:
  Pager() = delete;
  static Pager* InitPager(const char* filename);
  ~Pager();

  // Get a page from the data cache
  const char* GetPage(uint32_t page_num);
  // Write data to the data cache
  void Flush(uint32_t page_start);
};

}  // namespace storage

#endif  // SRC_PAGER_PAGER_H_
