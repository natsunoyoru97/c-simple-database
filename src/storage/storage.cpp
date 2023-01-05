// Copyright 2022 natsunoyoru97
//
// Created by natsunoyoru on 22-12-31.
//

#include "storage.h"  // NOLINT

#include <fcntl.h>
#include <unistd.h>

#include <cstdio>
#include <iostream>

// TODO(natsunoyoru97): duplicate declaration, this will be in an Util class
// The size of bytes a page has
constexpr uint32_t k_page_size = 256;
// The fixed-size of rows that contains in a page
constexpr uint32_t rows_per_page = 100;
// The size of bytes a row has
constexpr uint32_t row_size = 100;

namespace storage {

// TODO(natsunoyoru97): What's the use of the page_num here?
// A: the page_num should not be in the Table object.
Table::Table(const char* filename) {
  Pager* pager = Pager::InitPager(filename);
  pager_ = pager;
}

Table::~Table() {}

const char* Table::GetRowSlot(uint32_t row_num) {
  uint32_t page_num = row_num / rows_per_page;
  const char* page = pager_->GetPage(page_num);
  uint32_t row_offset = row_num % rows_per_page;
  uint32_t byte_offset = row_offset * row_size;

  return page + byte_offset;
}

}  // namespace storage
