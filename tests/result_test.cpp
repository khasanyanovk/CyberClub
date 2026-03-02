#include "core/result.hpp"
#include <gtest/gtest.h>
#include <string>

TEST(ResultTest, OkCreation) {
  auto result = Result<int, std::string>::ok(42);

  EXPECT_TRUE(result.is_ok());
  EXPECT_FALSE(result.is_err());
  EXPECT_EQ(result.value(), 42);
}

TEST(ResultTest, OkWithString) {
  auto result = Result<std::string, std::string>::ok("success");

  EXPECT_TRUE(result.is_ok());
  EXPECT_EQ(result.value(), "success");
}

TEST(ResultTest, ErrCreation) {
  auto result = Result<int, std::string>::err("something went wrong");

  EXPECT_FALSE(result.is_ok());
  EXPECT_TRUE(result.is_err());
  EXPECT_EQ(result.error(), "something went wrong");
}

TEST(ResultTest, ValueOrWithOk) {
  auto result = Result<int, std::string>::ok(42);

  EXPECT_EQ(result.value_or(0), 42);
}

TEST(ResultTest, ValueOrWithErr) {
  auto result = Result<int, std::string>::err("error");

  EXPECT_EQ(result.value_or(99), 99);
}

TEST(ResultTest, MoveValue) {
  auto result = Result<std::string, std::string>::ok("test");

  std::string value = std::move(result.value());
  EXPECT_EQ(value, "test");
}

TEST(ResultTest, CustomErrorType) {
  auto result = Result<int, int>::err(404);

  EXPECT_TRUE(result.is_err());
  EXPECT_EQ(result.error(), 404);
}
