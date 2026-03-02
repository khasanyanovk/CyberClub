#include "validator.hpp"
#include <gtest/gtest.h>

TEST(ValidatorTest, ValidClientNames) {
  EXPECT_TRUE(Validator::is_valid_client_name("client1"));
  EXPECT_TRUE(Validator::is_valid_client_name("client_name"));
  EXPECT_TRUE(Validator::is_valid_client_name("client-name"));
  EXPECT_TRUE(Validator::is_valid_client_name("abc123"));
  EXPECT_TRUE(Validator::is_valid_client_name("test_user-123"));
}

TEST(ValidatorTest, InvalidClientNames) {
  EXPECT_FALSE(Validator::is_valid_client_name(""));
  EXPECT_FALSE(Validator::is_valid_client_name("Client1"));
  EXPECT_FALSE(Validator::is_valid_client_name("client name"));
  EXPECT_FALSE(Validator::is_valid_client_name("client@name"));
  EXPECT_FALSE(Validator::is_valid_client_name("client.name"));
  EXPECT_FALSE(Validator::is_valid_client_name("клиент"));
}

TEST(ValidatorTest, ValidTableNumbers) {
  EXPECT_TRUE(Validator::is_valid_table_number(1));
  EXPECT_TRUE(Validator::is_valid_table_number(10));
  EXPECT_TRUE(Validator::is_valid_table_number(100));
}

TEST(ValidatorTest, InvalidTableNumbers) {
  EXPECT_FALSE(Validator::is_valid_table_number(0));
  EXPECT_FALSE(Validator::is_valid_table_number(-1));
  EXPECT_FALSE(Validator::is_valid_table_number(-100));
}

TEST(ValidatorTest, ValidTableNumbersWithRange) {
  EXPECT_TRUE(Validator::is_valid_table_number(1, 5));
  EXPECT_TRUE(Validator::is_valid_table_number(3, 5));
  EXPECT_TRUE(Validator::is_valid_table_number(5, 5));
}

TEST(ValidatorTest, InvalidTableNumbersWithRange) {
  EXPECT_FALSE(Validator::is_valid_table_number(0, 5));
  EXPECT_FALSE(Validator::is_valid_table_number(-1, 5));
  EXPECT_FALSE(Validator::is_valid_table_number(6, 5));
  EXPECT_FALSE(Validator::is_valid_table_number(10, 5));
}

TEST(ValidatorTest, ValidTablesCount) {
  EXPECT_TRUE(Validator::is_valid_tables_count(1));
  EXPECT_TRUE(Validator::is_valid_tables_count(10));
  EXPECT_TRUE(Validator::is_valid_tables_count(100));
}

TEST(ValidatorTest, InvalidTablesCount) {
  EXPECT_FALSE(Validator::is_valid_tables_count(0));
  EXPECT_FALSE(Validator::is_valid_tables_count(-1));
  EXPECT_FALSE(Validator::is_valid_tables_count(-100));
}

TEST(ValidatorTest, ValidCost) {
  EXPECT_TRUE(Validator::is_valid_cost(1));
  EXPECT_TRUE(Validator::is_valid_cost(10));
  EXPECT_TRUE(Validator::is_valid_cost(100));
}

TEST(ValidatorTest, InvalidCost) {
  EXPECT_FALSE(Validator::is_valid_cost(0));
  EXPECT_FALSE(Validator::is_valid_cost(-1));
  EXPECT_FALSE(Validator::is_valid_cost(-100));
}
