// Copyright 2022 natsunoyoru97
//
// Created by natsunoyoru on 22-12-31.
//

#include "storage.h"  // NOLINT
#include "../pager/pager.cpp"

namespace storage {
  
// TODO(natsunoyoru97): What's the use of the page_num here?
// A: the page_num should not be in the Table object.
Table* Table::InitTable(const char* filename) {
  Table* tbl = nullptr;
  Pager* pager = Pager::InitPager(filename);
  tbl->pager_ = pager;

  return tbl;
}

const char* Table::GetRowSlot(uint32_t row_num) {
  uint32_t page_num = row_num / rowsPerPage;
  const char* page = pager_->GetPage(page_num);
  uint32_t row_offset = row_num % rowsPerPage;
  uint32_t byte_offset = row_offset * rowSize;

  return page + byte_offset;
}

}  // namespace storage
