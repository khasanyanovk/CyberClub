#ifndef _RESULT_HPP_
#define _RESULT_HPP_

#include <optional>
#include <string>
#include <utility>

template <typename T, typename E = std::string> class Result {
private:
  std::optional<T> value_;
  std::optional<E> error_;

public:
  // Pass by value + move is better then reference when I want save value to
  // class field
  static Result ok(T value) {
    Result result;
    result.value_ = std::move(value);
    return result;
  }

  static Result err(E error) {
    Result result;
    result.error_ = std::move(error);
    return result;
  }

  [[nodiscard]] bool is_ok() const { return value_.has_value(); }
  [[nodiscard]] bool is_err() const { return error_.has_value(); }

  [[nodiscard]] const T &value() const { return value_.value(); }
  [[nodiscard]] T &value() { return value_.value(); }

  [[nodiscard]] const E &error() const { return error_.value(); }
  [[nodiscard]] E &error() { return error_.value(); }

  [[nodiscard]] T value_or(T defaultValue) const {
    return value_.value_or(std::move(defaultValue));
  }

private:
  Result() = default;
};

#endif
