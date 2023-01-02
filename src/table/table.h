// Copyright 2022 natsunoyoru97
//
// Created by natsunoyoru on 22-12-31.
//

#include <cstdint>

#ifndef SRC_TABLE_TABLE_H_
#define SRC_TABLE_TABLE_H_

const uint32_t TABLE_MAX_PAGES = 1024;

namespace table {

class Pager {
 public:
  // TODO(natsunoyoru97): considering making some of the member variables
  // private
  int file_descriptor;
  uint32_t file_len;
  void* pages[TABLE_MAX_PAGES];

  explicit Pager(const char* filename);
  ~Pager();

  void* GetPage(uint32_t page_num);
  void Flush(uint32_t page_num, uint32_t size);
};

class Table {
 public:
  // TODO(natsunoyoru97): considering making some of the member variables
  // private
  Pager* pager;
  uint32_t num_rows;

  explicit Table(const char* filename);
  ~Table();
  void* RowSlot(uint32_t row_num);
};

}  // namespace table

#endif  // SRC_TABLE_TABLE_H_
