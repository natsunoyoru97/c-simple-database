// Copyright 2022 natsunoyoru97

#include <gtest/gtest.h>

#include "../src/pager/pager.h"
#include "../src/storage/storage.h"

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
