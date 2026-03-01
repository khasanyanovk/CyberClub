#ifndef _IREADER_HPP_
#define _IREADER_HPP_

#include "result.hpp"
#include <string>
#include <vector>

class IReader {
public:
  using Output = std::vector<std::string>;
  using ReadResult = Result<Output, std::string>;

  virtual ~IReader() = default;

  [[nodiscard]] virtual ReadResult read() const = 0;
};

#endif