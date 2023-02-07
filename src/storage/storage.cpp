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

absl::StatusOr<const char*> Table::GetOneRowSlot(uint32_t start_page) {
  return pager_->GetPage(start_page);
}

}  // namespace storage
