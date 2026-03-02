#ifndef _IWRITER_HPP_
#define _IWRITER_HPP_

#include <string>

class IWriter {
public:
  virtual ~IWriter() = default;

  virtual void write_line(const std::string &line) = 0;
  virtual void flush() = 0;
};

#endif
