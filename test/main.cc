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
