// Copyright 2022 natsunoyoru97
//
// Created by natsunoyoru on 22-12-31.
//
#ifndef SRC_TABLE_TABLE_H_
#define SRC_TABLE_TABLE_H_

#include <cstdint>

#include "../pager/pager.h"

namespace table {

class Table {
 public:
  // TODO(natsunoyoru97): considering making some of the member variables
  // private
  pager::Pager* pager_;
  uint32_t num_rows_;

  explicit Table(const char* filename);
  ~Table();
  void* RowSlot(uint32_t row_num);
};

}  // namespace table

#endif  // SRC_TABLE_TABLE_H_
