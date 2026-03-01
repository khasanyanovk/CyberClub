#include "file_reader.hpp"
#include <fstream>

FileReader::FileReader(std::string filename) : filename_(std::move(filename)) {}

IReader::ReadResult FileReader::read() const {
  std::ifstream file(filename_);

  if (!file.is_open()) {
    return ReadResult::err("Can't open file: " + filename_);
  }

  Output lines;
  std::string line;

  while (std::getline(file, line)) {
    if (!line.empty() && line.back() == '\r') {
      line.pop_back();
    }
    lines.push_back(line);
  }

  if (file.bad()) {
    return ReadResult::err("Error reading file: " + filename_);
  }

  file.close();
  return ReadResult::ok(std::move(lines));
}
