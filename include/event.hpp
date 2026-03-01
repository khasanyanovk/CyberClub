#ifndef _EVENT_HPP_
#define _EVENT_HPP_

#include "time_point.hpp"
#include <cstdint>
#include <memory>
#include <string>

class Event {
protected:
  TimePoint time_;
  uint8_t id_;

public:
  // Pass by value because TimePoint has only one primary type field
  Event(TimePoint time, uint8_t event_id) : time_(time), id_(event_id) {}

  virtual ~Event() = default;

  TimePoint time() const { return time_; }
  int id() const { return id_; }

  virtual std::string to_string() const = 0;
};

using EventPtr = std::unique_ptr<Event>;

#endif
