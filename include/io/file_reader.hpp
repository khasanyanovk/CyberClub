#ifndef _FILE_READER_HPP_
#define _FILE_READER_HPP_

#include "io/ireader.hpp"
#include <string>

class FileReader : public IReader {
private:
  std::string filename_;

public:
  explicit FileReader(std::string filename);

  ReadResult read() const override;
};

#endif
