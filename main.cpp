#include "file_reader.hpp"
#include <iostream>
#include <memory>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
    return 1;
  }

  std::unique_ptr<IReader> reader = std::make_unique<FileReader>(argv[1]);

  auto result = reader->read();

  if (result.is_err()) {
    std::cerr << result.error() << std::endl;
    return 1;
  }

  const auto &lines = result.value();
  std::cout << "Successfully read " << lines.size() << " lines:" << std::endl;
  for (const auto &line : lines) {
    std::cout << line << std::endl;
  }

  return 0;
}