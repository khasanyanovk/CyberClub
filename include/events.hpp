#ifndef _EVENTS_HPP_
#define _EVENTS_HPP_

#include "event.hpp"

// ID 1
class ClientArrivedEvent : public Event {
private:
  std::string client_name_;

public:
  ClientArrivedEvent(TimePoint time, std::string client_name)
      : Event(time, 1), client_name_(std::move(client_name)) {}

  [[nodiscard]] const std::string &client_name() const { return client_name_; }

  [[nodiscard]] std::string to_string() const override {
    return time_.to_string() + " 1 " + client_name_;
  }
};

// ID 2
class ClientSatEvent : public Event {
private:
  std::string client_name_;
  int table_number_;

public:
  ClientSatEvent(TimePoint time, std::string client_name, int table_number)
      : Event(time, 2), client_name_(std::move(client_name)),
        table_number_(table_number) {}

  [[nodiscard]] const std::string &client_name() const { return client_name_; }
  [[nodiscard]] int table_number() const { return table_number_; }

  [[nodiscard]] std::string to_string() const override {
    return time_.to_string() + " 2 " + client_name_ + " " +
           std::to_string(table_number_);
  }
};

// ID 3
class ClientWaitingEvent : public Event {
private:
  std::string client_name_;

public:
  ClientWaitingEvent(TimePoint time, std::string client_name)
      : Event(time, 3), client_name_(std::move(client_name)) {}

  [[nodiscard]] const std::string &client_name() const { return client_name_; }

  [[nodiscard]] std::string to_string() const override {
    return time_.to_string() + " 3 " + client_name_;
  }
};

// ID 4
class ClientLeftEvent : public Event {
private:
  std::string client_name_;

public:
  ClientLeftEvent(TimePoint time, std::string client_name)
      : Event(time, 4), client_name_(std::move(client_name)) {}

  [[nodiscard]] const std::string &client_name() const { return client_name_; }

  [[nodiscard]] std::string to_string() const override {
    return time_.to_string() + " 4 " + client_name_;
  }
};

// ID 11
class ClientKickedEvent : public Event {
private:
  std::string client_name_;

public:
  ClientKickedEvent(TimePoint time, std::string client_name)
      : Event(time, 11), client_name_(std::move(client_name)) {}

  [[nodiscard]] const std::string &client_name() const { return client_name_; }

  [[nodiscard]] std::string to_string() const override {
    return time_.to_string() + " 11 " + client_name_;
  }
};

// ID 12
class ClientSeatedEvent : public Event {
private:
  std::string client_name_;
  int table_number_;

public:
  ClientSeatedEvent(TimePoint time, std::string client_name, int table_number)
      : Event(time, 12), client_name_(std::move(client_name)),
        table_number_(table_number) {}

  [[nodiscard]] const std::string &client_name() const { return client_name_; }
  [[nodiscard]] int table_number() const { return table_number_; }

  [[nodiscard]] std::string to_string() const override {
    return time_.to_string() + " 12 " + client_name_ + " " +
           std::to_string(table_number_);
  }
};

// ID 13
class ErrorEvent : public Event {
private:
  std::string error_message_;

public:
  ErrorEvent(TimePoint time, std::string error_message)
      : Event(time, 13), error_message_(std::move(error_message)) {}

  [[nodiscard]] const std::string &error_message() const {
    return error_message_;
  }

  [[nodiscard]] std::string to_string() const override {
    return time_.to_string() + " 13 " + error_message_;
  }
};

#endif
