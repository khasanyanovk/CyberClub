#include "file_reader.hpp"
#include "parser.hpp"
#include <iostream>
#include <memory>
#include <vector>

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

  size_t start_line = 3;

  std::vector<EventPtr> events;
  for (size_t i = start_line; i < lines.size(); ++i) {
    auto event_result = Parser::parse_event(lines[i]);

    if (event_result.is_err()) {
      std::cerr << event_result.error() << std::endl;
      return 1;
    }

    events.push_back(std::move(event_result.value()));
  }

  std::cout << "Parsed " << events.size() << " events:" << std::endl;
  for (const auto &event : events) {
    std::cout << event->to_string() << std::endl;
  }

  return 0;
}