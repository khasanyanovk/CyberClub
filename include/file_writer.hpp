#ifndef _FILE_WRITER_HPP_
#define _FILE_WRITER_HPP_

#include "iwriter.hpp"
#include <fstream>
#include <string>

class FileWriter : public IWriter {
private:
  std::ofstream file_;

public:
  explicit FileWriter(const std::string &filename);
  ~FileWriter() override;

  void write_line(const std::string &line) override;
  void flush() override;

  [[nodiscard]] bool is_open() const;
};

#endif
