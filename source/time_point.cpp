#include "time_point.hpp"
#include <iomanip>
#include <sstream>

TimePoint::TimePoint() : minutes_(0) {}

TimePoint::TimePoint(int hours, int minutes) : minutes_(hours * 60 + minutes) {}

TimePoint::TimePoint(std::chrono::minutes minutes) : minutes_(minutes) {}

bool TimePoint::operator<(const TimePoint &other) const {
  return minutes_ < other.minutes_;
}

bool TimePoint::operator<=(const TimePoint &other) const {
  return minutes_ <= other.minutes_;
}

bool TimePoint::operator>(const TimePoint &other) const {
  return minutes_ > other.minutes_;
}

bool TimePoint::operator>=(const TimePoint &other) const {
  return minutes_ >= other.minutes_;
}

bool TimePoint::operator==(const TimePoint &other) const {
  return minutes_ == other.minutes_;
}

std::string TimePoint::to_string() const {
  std::ostringstream oss;
  oss << std::setfill('0') << std::setw(2) << hours() << ":"
      << std::setfill('0') << std::setw(2) << minutes();
  return oss.str();
}
