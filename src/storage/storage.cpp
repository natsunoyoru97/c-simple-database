// Copyright 2022 natsunoyoru97
//
// Created by natsunoyoru on 22-12-31.
//

#include "storage.h"  // NOLINT

#include <cstdint>

namespace storage {

Table::Table(Pager* pager) { pager_ = pager; }

absl::StatusOr<Table*> Table::InitTable(const char* filename) {
  absl::StatusOr<Pager*> result = Pager::InitPager(filename);
  return result.ok() ? absl::StatusOr<Table*>(new Table(*result))
                     : absl::StatusOr<Table*>(
                           absl::AbortedError("Failed to init Pager"));
}

Pager* Table::GetPager() { return pager_; }

Table::~Table() { delete pager_; }

absl::StatusOr<const char*> Table::GetRowSlot(uint32_t row_num) {
  uint32_t page_num = row_num / rowsPerPage;
  absl::StatusOr<const char*> page = pager_->GetPage(page_num);
  if (!page.ok()) {
    return page;
  }
  
  uint32_t row_offset = row_num % rowsPerPage;
  uint32_t byte_offset = row_offset * rowSize;

  return absl::StatusOr<const char*>(*page + byte_offset);
}

}  // namespace storage
