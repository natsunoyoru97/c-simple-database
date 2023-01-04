// Copyright 2022 natsunoyoru97
//
// Created by natsunoyoru on 22-12-31.
//
#ifndef SRC_STORAGE_STORAGE_H_
#define SRC_STORAGE_STORAGE_H_

#include <cstdint>

#include "../pager/pager.h"

namespace storage {

// Storage manager of the database - 
// A wrapper to maintain the database files 
// and it only provides a READ interface 
class Table {
 private:
  pager::Pager* pager_;
 public:
  explicit Table(const char* filename);
  ~Table();
  // A getter of a row of a page
  // It can only READ the data from a centrain page
  void* GetRowSlot(uint32_t row_num);
};

}  // namespace storage

#endif  // SRC_STORAGE_STORAGE_H_