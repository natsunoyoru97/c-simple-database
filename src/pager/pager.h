// Copyright 2022 natsunoyoru97
//
// Created by natsunoyoru on 23-1-2.
//

#ifndef SRC_PAGER_PAGER_H_
#define SRC_PAGER_PAGER_H_

#include <array>
#include <cstdint>

// Maximum pages a table stores
constexpr uint32_t TABLE_MAX_PAGES = 1024;

namespace storage {

// The indirection layer to handle file descriptor and give a block of memory
class Pager {
 private:
  static int fd_;
  static uint32_t file_len_;
  // Rows this block of memory has
  // NOTE(natsunoyoru97): I would like to remain this member variable for a
  // while to see if it is in need for future
  static uint32_t num_rows_;
  // TODO(natsunoyoru97): how about using other data structures?
  static std::array<char*, TABLE_MAX_PAGES> pages_;

 public:
  static Pager* InitPager(const char* filename);
  Pager() = delete;
  ~Pager();

  // Get a page from the data cache
  const char* GetPage(uint32_t page_num);
  // Write data to the data cache
  void Flush(uint32_t page_start, uint32_t page_num_to_flush);
};

}  // namespace storage

#endif  // SRC_PAGER_PAGER_H_
