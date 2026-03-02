#include "club_manager.hpp"
#include "club.hpp"
#include "parser.hpp"
#include <iomanip>
#include <sstream>

ClubManager::ClubManager(std::unique_ptr<IReader> reader,
                         std::unique_ptr<IWriter> writer)
    : reader_(std::move(reader)), writer_(std::move(writer)) {}

void ClubManager::write_output(const std::string &line) {
  writer_->write_line(line);
}

Result<void, std::string> ClubManager::run() {
  auto read_result = reader_->read();
  if (read_result.is_err()) {
    return Result<void, std::string>::err(read_result.error());
  }

  const auto &lines = read_result.value();

  if (lines.size() < 3) {
    return Result<void, std::string>::err("Not enough lines in input file");
  }

  std::vector<std::string> config_lines(lines.begin(), lines.begin() + 3);
  auto config_result = Parser::parse_config(config_lines);

  if (config_result.is_err()) {
    return Result<void, std::string>::err(config_result.error());
  }

  club_config config = config_result.value();
  Club club(config);

  write_output(club.open_time().to_string());

  for (size_t i = 3; i < lines.size(); ++i) {
    auto event_result = Parser::parse_event(lines[i]);

    if (event_result.is_err()) {
      return Result<void, std::string>::err(event_result.error());
    }

    auto output = club.process_event(std::move(event_result.value()));

    for (const auto &event : output) {
      write_output(event->to_string());
    }
  }

  auto close_events = club.close_club();
  for (const auto &event : close_events) {
    write_output(event->to_string());
  }

  write_output(club.close_time().to_string());

  const auto &stats = club.get_stats();
  for (size_t i = 0; i < stats.size(); ++i) {
    int table_num = static_cast<int>(i + 1);
    int hours = stats[i].total_minutes / 60;
    int minutes = stats[i].total_minutes % 60;

    std::ostringstream oss;
    oss << table_num << " " << stats[i].revenue << " " << std::setfill('0')
        << std::setw(2) << hours << ":" << std::setw(2) << minutes;
    write_output(oss.str());
  }

  writer_->flush();
  return Result<void, std::string>::ok();
}
