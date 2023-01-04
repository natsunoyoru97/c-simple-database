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
// TODO(natsunoyoru97): duplicate declaration, this will be in an Util class
constexpr uint32_t PAGE_SIZE = 256;

namespace storage {

// TODO(natsunoyoru97): What's the use of the page_num here? Why we have Table and Pager here?
Table::Table(const char* filename) {
  pager::Pager* pager = new pager::Pager(filename);
  pager_ = pager;
}

Table::~Table() {}

// NOTE(natsunoyoru97): This is assumed to be a *getter* 
void* Table::GetRowSlot(uint32_t row_num) {
  uint32_t page_num = row_num / ROWS_PER_PAGE;
  void* page = pager_->GetPage(page_num);
  uint32_t row_offset = row_num % ROWS_PER_PAGE;
  uint32_t byte_offset = row_offset * ROW_SIZE;

  return page + byte_offset;
}

}  // namespace storage
