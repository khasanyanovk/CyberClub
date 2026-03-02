#include "club_manager.hpp"
#include "file_reader.hpp"
#include "stream_writer.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

const char *EXPECTED_OUTPUT_FILE = "examples/test_output.txt";

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
    return 1;
  }

  std::ostringstream output;

  auto reader = std::make_unique<FileReader>(argv[1]);
  auto writer = std::make_unique<StreamWriter>(output);

  ClubManager manager(std::move(reader), std::move(writer));
  auto result = manager.run();

  if (result.is_err()) {
    std::cerr << result.error() << std::endl;
    return 1;
  }

  std::string actual_output = output.str();
  std::cout << actual_output;

  std::ifstream expected_file(EXPECTED_OUTPUT_FILE);
  if (expected_file.is_open()) {
    std::stringstream buffer;
    buffer << expected_file.rdbuf();
    std::string expected_output = buffer.str();
    expected_file.close();

    return (actual_output != expected_output);
  } else {
    std::cerr << "\nError: Could not open " << EXPECTED_OUTPUT_FILE
              << " for comparison" << std::endl;
    return 1;
  }

  return 0;
}