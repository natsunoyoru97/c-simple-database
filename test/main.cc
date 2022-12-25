// Copyright 2022 natsunoyoru97

#include <gtest/gtest.h>

TEST(DatabaseTest, EchoWorks) { EXPECT_STRNE("hello", "world"); }

TEST(DatabaseTest, AddWorks) { EXPECT_STREQ("hello", "hello"); }

// Tests for query execution

TEST(QueryTest, SelectWorks) {}

TEST(QueryTest, InsertWorks) {}

TEST(QueryTest, DeleteWorks) {}
