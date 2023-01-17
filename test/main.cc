// Copyright 2022 natsunoyoru97
#include <fcntl.h>
#include <gtest/gtest.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <cstdio>

#include "absl/status/status.h"
#include "absl/status/statusor.h"

TEST(DatabaseTest, EchoWorks) { EXPECT_STRNE("hello", "world"); }

TEST(DatabaseTest, AddWorks) { EXPECT_STREQ("hello", "hello"); }

// Test for Utils
absl::Status ReturnWithStatus(bool b) {
  return (b) ? absl::OkStatus() : absl::InvalidArgumentError("b is not true");
}

TEST(UtilsTest, StatusWorks) {
  EXPECT_EQ(ReturnWithStatus(true).ok(), true);
  EXPECT_NE(ReturnWithStatus(false).ok(), true);
}

TEST(UtilsTest, StatusOrWorks) {}

// Tests for query execution

TEST(QueryTest, SelectWorks) {}

TEST(QueryTest, InsertWorks) {}

TEST(QueryTest, DeleteWorks) {}
