// Copyright 2022 natsunoyoru97

#include <gtest/gtest.h>

#include "../src/pager/pager.h"
#include "../src/storage/storage.h"

TEST(DatabaseTest, EchoWorks) { EXPECT_STRNE("hello", "world"); }

TEST(DatabaseTest, AddWorks) { EXPECT_STREQ("hello", "hello"); }

// Tests for query execution

TEST(QueryTest, SelectWorks) {}

TEST(QueryTest, InsertWorks) {}

TEST(QueryTest, DeleteWorks) {}

// Tests for Storage
void init() {
  storage::Table* tbl = storage::Table::InitTable("./basic.db");
  std::cerr << sizeof(tbl) << "\n";
  tbl->GetRowSlot(1);
  delete tbl;
}

TEST(StorageTest, CtorWorks) {
  init();
}

TEST(StorageTest, DtorWorks) {}
