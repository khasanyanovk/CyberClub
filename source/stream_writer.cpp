#include "stream_writer.hpp"

StreamWriter::StreamWriter(std::ostream &stream) : stream_(stream) {}

void StreamWriter::write_line(const std::string &line) {
  stream_ << line << '\n';
}

void StreamWriter::flush() { stream_.flush(); }
