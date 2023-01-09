// Copyright 2022 natsunoyoru97

#include <gtest/gtest.h>
#include "../src/storage/storage.cpp"

TEST(DatabaseTest, EchoWorks) { EXPECT_STRNE("hello", "world"); }

TEST(DatabaseTest, AddWorks) { EXPECT_STREQ("hello", "hello"); }

// Tests for query execution

TEST(QueryTest, SelectWorks) {}

TEST(QueryTest, InsertWorks) {}

TEST(QueryTest, DeleteWorks) {}

// Tests for Storage
TEST(StorageTest, CtorWorks) {
    storage::Table* tbl = storage::Table::InitTable("./basic.db");
    tbl->GetRowSlot(1);
}

TEST(StorageTest, DtorWorks) {}
