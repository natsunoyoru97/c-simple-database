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

namespace pager {

// The indirection layer to handle file descriptor and give a block of memory
class Pager {
 private:
  int fd_;
  uint32_t file_len_;
  // Rows this block of memory has
  uint32_t num_rows_;
  // TODO(natsunoyoru97): how about using other data structures?
  std::array<void*, TABLE_MAX_PAGES> pages_;

 public:
  explicit Pager(const char* filename);
  ~Pager();

  // Get a page from the data cache
  void* GetPage(uint32_t page_num);
  // Write data to the data cache
  void Flush(uint32_t page_num, uint32_t size);
};

}  // namespace pager

#endif  // SRC_PAGER_PAGER_H_
