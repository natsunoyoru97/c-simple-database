// Copyright 2022 natsunoyoru97

#include <gtest/gtest.h>
#include "../src/storage/storage.h"

TEST(DatabaseTest, EchoWorks) { EXPECT_STRNE("hello", "world"); }

TEST(DatabaseTest, AddWorks) { EXPECT_STREQ("hello", "hello"); }

// Tests for query execution

TEST(QueryTest, SelectWorks) {}

TEST(QueryTest, InsertWorks) {}

TEST(QueryTest, DeleteWorks) {}

// Tests for Storage
TEST(StorageTest, CtorWorks) {
    storage::Table tbl = new storage::InitTable("../datafile/basic.db");
}

TEST(StorageTest, DtorWorks) {}

// Tests for Pager
TEST(PagerTest, CtorWorks) {}