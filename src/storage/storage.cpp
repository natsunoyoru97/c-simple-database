// Copyright 2022 natsunoyoru97
//
// Created by natsunoyoru on 22-12-31.
//

// TODO(natsunoyoru97): the NOLINT should be removed
#include "storage.h"  // NOLINT

namespace storage {

Table::Table(const char* filename) { pager_ = Pager::InitPager(filename); }

Table* Table::InitTable(const char* filename) { return new Table(filename); }

Pager* Table::GetPager() { return pager_; }

Table::~Table() { delete pager_; }

const char* Table::GetRowSlot(uint32_t row_num) {
  uint32_t page_num = row_num / rowsPerPage;
  const char* page = pager_->GetPage(page_num);
  uint32_t row_offset = row_num % rowsPerPage;
  uint32_t byte_offset = row_offset * rowSize;

  return page + byte_offset;
}

void Table::SetRowSlot(uint32_t row_num) {
  uint32_t page_num = row_num / rowsPerPage;
  /*
  const char* page = pager_->GetPage(page_num);
  uint32_t row_offset = row_num % rowsPerPage;
  uint32_t byte_offset = row_offset * rowSize;
  */
  pager_->Flush(page_num);
}

}  // namespace storage
