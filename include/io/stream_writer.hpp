#ifndef _STREAM_WRITER_HPP_
#define _STREAM_WRITER_HPP_

#include "io/iwriter.hpp"
#include <ostream>

class StreamWriter : public IWriter {
private:
  std::ostream &stream_;

public:
  explicit StreamWriter(std::ostream &stream);

  void write_line(const std::string &line) override;
  void flush() override;
};

#endif
