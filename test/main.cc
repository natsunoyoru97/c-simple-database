// Copyright 2022 natsunoyoru97
#include <gtest/gtest.h>

#include <cstdio>

#include "absl/status/status.h"

TEST(DatabaseTest, EchoWorks) { EXPECT_STRNE("hello", "world"); }

TEST(DatabaseTest, AddWorks) { EXPECT_STREQ("hello", "hello"); }

// Test for Utils
TEST(UtilsTest, StatusWorks) {
  absl::Status stat = open("basic.db");
  std::cerr << stat.ok() << "\n";
}

// Tests for query execution

TEST(QueryTest, SelectWorks) {}

TEST(QueryTest, InsertWorks) {}

TEST(QueryTest, DeleteWorks) {}
