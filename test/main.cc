// Copyright 2022 natsunoyoru97
#include <fcntl.h>
#include <gtest/gtest.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <cstdint>
#include <cstdio>
#include <thread>

#include "../src/pager/pager.h"
#include "../src/storage/storage.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"

// Test for Utils
absl::Status IsBoolTrue(bool b) {
  return (b) ? absl::OkStatus() : absl::InvalidArgumentError("b is not true.");
}

absl::StatusOr<std::string> IsStringAlpha(std::string s) {
  return std::all_of(s.cbegin(), s.cend(), isalpha)
             ? absl::StatusOr<std::string>(s)
             : absl::StatusOr<std::string>(absl::InvalidArgumentError(
                   "the string has non-alpha characters."));
}

TEST(UtilsTest, StatusWorks) {
  EXPECT_TRUE(IsBoolTrue(true).ok());
  EXPECT_FALSE(IsBoolTrue(false).ok());
}

TEST(UtilsTest, StatusOrWorks) {
  EXPECT_TRUE(IsStringAlpha("abc").ok());
  EXPECT_FALSE(IsStringAlpha("abc1").ok());
}

// Tests for query execution

TEST(QueryTest, SelectWorks) {}

TEST(QueryTest, InsertWorks) {}

TEST(QueryTest, DeleteWorks) {}

// Tests for FileHandler

TEST(FileHandlerTest, OpenFileWorks) {
  absl::StatusOr<storage::Table*> result =
      storage::Table::InitTable("./basic.db");
  EXPECT_TRUE(result.ok());

  storage::Table* tbl = *result;
  delete tbl;
}

TEST(FileHandlerTest, OpenFileErrorWorks) {
  absl::StatusOr<storage::Table*> result =
      storage::Table::InitTable("../datafile/notExists.db");
  EXPECT_FALSE(result.ok());
}

// Tests for Storage

TEST(StorageTest, GetRowSlotWorks) {
  absl::StatusOr<storage::Table*> result =
      storage::Table::InitTable("./basic.db");
  EXPECT_TRUE(result.ok());

  storage::Table* tbl = *result;
  absl::StatusOr<const char*> page = tbl->GetRowSlot(100);
  EXPECT_TRUE(page.ok());
  EXPECT_EQ(sizeof(page), 8);

  delete tbl;
}

TEST(StorageTest, IncRowCntAtomicWorks) {
  absl::StatusOr<storage::Table*> result =
      storage::Table::InitTable("./basic.db");
  EXPECT_TRUE(result.ok());

  storage::Table* tbl = *result;
  storage::Pager* pager = tbl->GetPager();
  uint32_t cnt = pager->GetNumRows();

  for (int i = 0; i < 10000; ++i) {
    std::thread t1(&storage::Pager::IncRowCntByOne, pager);
    std::thread t2(&storage::Pager::IncRowCntByOne, pager);

    t1.join();
    t2.join();

    if (std::this_thread::get_id() == t1.get_id() ||
        std::this_thread::get_id() == t2.get_id()) {
      EXPECT_EQ(++cnt, pager->GetNumRows());
    }
  }

  delete tbl;
}
