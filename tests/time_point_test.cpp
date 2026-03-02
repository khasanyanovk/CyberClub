#include "core/time_point.hpp"
#include <gtest/gtest.h>

TEST(TimePointTest, ConstructorAndGetters) {
  TimePoint tp(10, 30);

  EXPECT_EQ(tp.hours(), 10);
  EXPECT_EQ(tp.minutes(), 30);
}

TEST(TimePointTest, ToString) {
  TimePoint tp(9, 5);

  EXPECT_EQ(tp.to_string(), "09:05");
}

TEST(TimePointTest, Comparison) {
  TimePoint tp1(10, 30);
  TimePoint tp2(11, 0);
  TimePoint tp3(10, 30);

  EXPECT_LT(tp1, tp2);
  EXPECT_EQ(tp1, tp3);
  EXPECT_GT(tp2, tp1);
}
