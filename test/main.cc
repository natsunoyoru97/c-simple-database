// Copyright 2022 natsunoyoru97
#include <fcntl.h>
#include <gtest/gtest.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <cstdio>

#include "absl/status/status.h"
#include "absl/status/statusor.h"

#include "../src/pager/pager.h"
#include "../src/storage/storage.h"

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
  storage::Table* tbl = storage::Table::InitTable("basic.db");
  int fd = tbl->GetPager()->GetFileHandler()->GetFd();
  EXPECT_NE(fd, -1);
  delete tbl;
}

TEST(FileHandlerTest, OpenFileErrorWorks) {
  storage::Table* tbl = storage::Table::InitTable("../datafile/notExists.db");
  int fd = tbl->GetPager()->GetFileHandler()->GetFd();
  EXPECT_EQ(fd, -1);
  delete tbl;
}

// Tests for Storage

TEST(StorageTest, GetRowSlotWorks) {
  storage::Table* tbl = storage::Table::InitTable("./basic.db");
  const char* page = tbl->GetRowSlot(1);
  EXPECT_EQ(sizeof(page), 8);
  delete tbl;
}
