// Copyright 2022 natsunoyoru97
//
// Created by natsunoyoru on 22-12-31.
//

#include "storage.h"

#include <fcntl.h>
#include <unistd.h>

#include <cstdio>
#include <iostream>

constexpr uint32_t ROWS_PER_PAGE = 100;
constexpr uint32_t ROW_SIZE = 100;
// TODO(natsunoyoru): duplicate declaration, this will be in an Util class
constexpr uint32_t PAGE_SIZE = 256;

namespace storage {

Table::Table(const char* filename) {
  pager::Pager* pager = new pager::Pager(filename);
  uint32_t num_rows = pager->file_len_ / ROW_SIZE;

  pager_ = pager;
  num_rows_ = num_rows;
}

Table::~Table() {
  uint32_t num_full_pages = num_rows_ / ROWS_PER_PAGE;

  for (uint32_t i = 0; i < num_full_pages; ++i) {
    if (pager_->pages_[i] == nullptr) {
      continue;
    }
    pager_->Flush(i, PAGE_SIZE);
    // NOTE(natsunoyoru97): void* cannot apply for new[]/delete[]
    free(pager_->pages_[i]);
    pager_->pages_[i] = nullptr;
  }

  // There may be a partial page to write to the end of the file
  uint32_t num_additional_rows = num_rows_ % ROWS_PER_PAGE;
  if (num_additional_rows > 0) {
    uint32_t page_num = num_full_pages;
    if (pager_->pages_[page_num] != NULL) {
      pager_->Flush(page_num, num_additional_rows * ROW_SIZE);
      // NOTE(natsunoyoru97): void* cannot apply for new[]/delete[]
      free(pager_->pages_[page_num]);
      pager_->pages_[page_num] = NULL;
    }
  }
}

void* Table::RowSlot(uint32_t row_num) {
  uint32_t page_num = row_num / ROWS_PER_PAGE;
  void* page = pager_->GetPage(page_num);
  uint32_t row_offset = row_num % ROWS_PER_PAGE;
  uint32_t byte_offset = row_offset * ROW_SIZE;

  return page + byte_offset;
}

}  // namespace storage
