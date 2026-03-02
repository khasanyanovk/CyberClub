#include "file_writer.hpp"

FileWriter::FileWriter(const std::string &filename) : file_(filename) {}

FileWriter::~FileWriter() {
  if (file_.is_open()) {
    file_.close();
  }
}

void FileWriter::write_line(const std::string &line) {
  if (file_.is_open()) {
    file_ << line << '\n';
  }
}

void FileWriter::flush() {
  if (file_.is_open()) {
    file_.flush();
  }
}

bool FileWriter::is_open() const { return file_.is_open(); }
