#include "domain/parser.hpp"
#include <gtest/gtest.h>

TEST(ParserTest, ParseTimeValid) {
  auto result = Parser::parse_time("09:30");

  ASSERT_TRUE(result.is_ok());
  EXPECT_EQ(result.value().hours(), 9);
  EXPECT_EQ(result.value().minutes(), 30);
}

TEST(ParserTest, ParseTimeWithLeadingZeros) {
  auto result = Parser::parse_time("00:00");

  ASSERT_TRUE(result.is_ok());
  EXPECT_EQ(result.value().hours(), 0);
  EXPECT_EQ(result.value().minutes(), 0);
}

TEST(ParserTest, ParseTimeMaxValue) {
  auto result = Parser::parse_time("23:59");

  ASSERT_TRUE(result.is_ok());
  EXPECT_EQ(result.value().hours(), 23);
  EXPECT_EQ(result.value().minutes(), 59);
}

TEST(ParserTest, ParseTimeInvalidFormat) {
  auto result1 = Parser::parse_time("9:30");
  auto result2 = Parser::parse_time("09:3");
  auto result3 = Parser::parse_time("9:3");
  auto result4 = Parser::parse_time("09-30");
  auto result5 = Parser::parse_time("abc:def");

  EXPECT_TRUE(result1.is_err());
  EXPECT_TRUE(result2.is_err());
  EXPECT_TRUE(result3.is_err());
  EXPECT_TRUE(result4.is_err());
  EXPECT_TRUE(result5.is_err());
}

TEST(ParserTest, ParseTimeInvalidHours) {
  auto result = Parser::parse_time("24:00");

  EXPECT_TRUE(result.is_err());
}

TEST(ParserTest, ParseTimeInvalidMinutes) {
  auto result = Parser::parse_time("12:60");

  EXPECT_TRUE(result.is_err());
}

TEST(ParserTest, ParseClientArrivedEvent) {
  auto result = Parser::parse_event("09:30 1 client1");

  ASSERT_TRUE(result.is_ok());
  auto event = std::move(result.value());

  EXPECT_EQ(event->id(), 1);
  EXPECT_EQ(event->time().hours(), 9);
  EXPECT_EQ(event->time().minutes(), 30);
  EXPECT_EQ(event->to_string(), "09:30 1 client1");
}

TEST(ParserTest, ParseClientSatEvent) {
  auto result = Parser::parse_event("10:25 2 client2 3");

  ASSERT_TRUE(result.is_ok());
  auto event = std::move(result.value());

  EXPECT_EQ(event->id(), 2);
  EXPECT_EQ(event->to_string(), "10:25 2 client2 3");
}

TEST(ParserTest, ParseClientWaitingEvent) {
  auto result = Parser::parse_event("11:45 3 client3");

  ASSERT_TRUE(result.is_ok());
  auto event = std::move(result.value());

  EXPECT_EQ(event->id(), 3);
  EXPECT_EQ(event->to_string(), "11:45 3 client3");
}

TEST(ParserTest, ParseClientLeftEvent) {
  auto result = Parser::parse_event("12:33 4 client4");

  ASSERT_TRUE(result.is_ok());
  auto event = std::move(result.value());

  EXPECT_EQ(event->id(), 4);
  EXPECT_EQ(event->to_string(), "12:33 4 client4");
}

TEST(ParserTest, ParseEventInvalidFormat) {
  auto result1 = Parser::parse_event("invalid");
  auto result2 = Parser::parse_event("09:30");
  auto result3 = Parser::parse_event("09:30 1");
  auto result4 = Parser::parse_event("09:30 2 client1");

  EXPECT_TRUE(result1.is_err());
  EXPECT_TRUE(result2.is_err());
  EXPECT_TRUE(result3.is_err());
  EXPECT_TRUE(result4.is_err());
}

TEST(ParserTest, ParseEventInvalidId) {
  auto result = Parser::parse_event("09:30 99 client1");

  EXPECT_TRUE(result.is_err());
}

TEST(ParserTest, ParseConfigValid) {
  std::vector<std::string> lines = {"3", "09:00 19:00", "10"};
  auto result = Parser::parse_config(lines);

  ASSERT_TRUE(result.is_ok());
  EXPECT_EQ(result.value().tables_count, 3);
  EXPECT_EQ(result.value().open_time.hours(), 9);
  EXPECT_EQ(result.value().open_time.minutes(), 0);
  EXPECT_EQ(result.value().close_time.hours(), 19);
  EXPECT_EQ(result.value().close_time.minutes(), 0);
  EXPECT_EQ(result.value().cost_per_hour, 10);
}

TEST(ParserTest, ParseConfigInvalidTableCount) {
  std::vector<std::string> lines1 = {"0", "09:00 19:00", "10"};
  std::vector<std::string> lines2 = {"-5", "09:00 19:00", "10"};
  std::vector<std::string> lines3 = {"abc", "09:00 19:00", "10"};

  EXPECT_TRUE(Parser::parse_config(lines1).is_err());
  EXPECT_TRUE(Parser::parse_config(lines2).is_err());
  EXPECT_TRUE(Parser::parse_config(lines3).is_err());
}

TEST(ParserTest, ParseConfigInvalidTimeFormat) {
  std::vector<std::string> lines1 = {"3", "9:00 19:00", "10"};
  std::vector<std::string> lines2 = {"3", "09:00 25:00", "10"};
  std::vector<std::string> lines3 = {"3", "09:00", "10"};

  EXPECT_TRUE(Parser::parse_config(lines1).is_err());
  EXPECT_TRUE(Parser::parse_config(lines2).is_err());
  EXPECT_TRUE(Parser::parse_config(lines3).is_err());
}

TEST(ParserTest, ParseConfigInvalidTimeOrder) {
  std::vector<std::string> lines1 = {"3", "19:00 09:00", "10"};
  std::vector<std::string> lines2 = {"3", "09:00 09:00", "10"};

  EXPECT_TRUE(Parser::parse_config(lines1).is_err());
  EXPECT_TRUE(Parser::parse_config(lines2).is_err());
}

TEST(ParserTest, ParseConfigInvalidCost) {
  std::vector<std::string> lines1 = {"3", "09:00 19:00", "0"};
  std::vector<std::string> lines2 = {"3", "09:00 19:00", "-10"};
  std::vector<std::string> lines3 = {"3", "09:00 19:00", "abc"};

  EXPECT_TRUE(Parser::parse_config(lines1).is_err());
  EXPECT_TRUE(Parser::parse_config(lines2).is_err());
  EXPECT_TRUE(Parser::parse_config(lines3).is_err());
}
