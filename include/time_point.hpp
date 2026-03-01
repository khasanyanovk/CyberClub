#ifndef _TIME_POINT_HPP_
#define _TIME_POINT_HPP_

#include <chrono>
#include <string>

class TimePoint {
private:
  std::chrono::minutes minutes_;

public:
  TimePoint();
  TimePoint(int hours, int minutes);
  explicit TimePoint(std::chrono::minutes minutes);

  // Defined here for inline
  [[nodiscard]] int hours() const {
    return static_cast<int>(minutes_.count() / 60);
  }

  [[nodiscard]] int minutes() const {
    return static_cast<int>(minutes_.count() % 60);
  }

  [[nodiscard]] std::chrono::minutes to_minutes() const { return minutes_; }

  bool operator<(const TimePoint &) const;
  bool operator<=(const TimePoint &) const;
  bool operator>(const TimePoint &) const;
  bool operator>=(const TimePoint &) const;
  bool operator==(const TimePoint &) const;

  [[nodiscard]] std::string to_string() const;
};

#endif
