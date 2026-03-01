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

  bool is_ok() const { return value_.has_value(); }
  bool is_err() const { return error_.has_value(); }

  const T &value() const { return value_.value(); }
  T &value() { return value_.value(); }

  const E &error() const { return error_.value(); }
  E &error() { return error_.value(); }

  T value_or(T defaultValue) const {
    return value_.value_or(std::move(defaultValue));
  }

private:
  Result() = default;
};

#endif
