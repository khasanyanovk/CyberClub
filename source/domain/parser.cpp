#include "domain/parser.hpp"
#include "domain/validator.hpp"
#include "events/events.hpp"
#include <functional>
#include <regex>
#include <sstream>
#include <unordered_map>


Result<TimePoint, std::string> Parser::parse_time(const std::string &time_str) {
  std::regex time_regex(R"(^(\d{2}):(\d{2})$)");
  std::smatch match;

  if (!std::regex_match(time_str, match, time_regex)) {
    return Result<TimePoint, std::string>::err("Invalid time format: " +
                                               time_str);
  }

  int hours = std::stoi(match[1].str());
  int minutes = std::stoi(match[2].str());

  if (hours < 0 || hours > 23) {
    return Result<TimePoint, std::string>::err("Invalid hours: " +
                                               std::to_string(hours));
  }

  if (minutes < 0 || minutes > 59) {
    return Result<TimePoint, std::string>::err("Invalid minutes: " +
                                               std::to_string(minutes));
  }

  return Result<TimePoint, std::string>::ok(TimePoint(hours, minutes));
}

Result<club_config, std::string>
Parser::parse_config(const std::vector<std::string> &lines) {
  if (lines.size() < 3) {
    return Result<club_config, std::string>::err("Not enough lines for config");
  }

  club_config config;

  try {
    config.tables_count = std::stoi(lines[0]);
    if (!Validator::is_valid_tables_count(config.tables_count)) {
      return Result<club_config, std::string>::err(lines[0]);
    }
  } catch (...) {
    return Result<club_config, std::string>::err(lines[0]);
  }

  std::istringstream iss(lines[1]);
  std::string open_str, close_str;
  if (!(iss >> open_str >> close_str)) {
    return Result<club_config, std::string>::err(lines[1]);
  }

  auto open_result = Parser::parse_time(open_str);
  if (open_result.is_err()) {
    return Result<club_config, std::string>::err(lines[1]);
  }
  config.open_time = open_result.value();

  auto close_result = Parser::parse_time(close_str);
  if (close_result.is_err()) {
    return Result<club_config, std::string>::err(lines[1]);
  }
  config.close_time = close_result.value();

  if (config.close_time <= config.open_time) {
    return Result<club_config, std::string>::err(lines[1]);
  }

  try {
    config.cost_per_hour = std::stoi(lines[2]);
    if (!Validator::is_valid_cost(config.cost_per_hour)) {
      return Result<club_config, std::string>::err(lines[2]);
    }
  } catch (...) {
    return Result<club_config, std::string>::err(lines[2]);
  }

  return Result<club_config, std::string>::ok(config);
}

template <typename EventType>
Result<EventPtr, std::string> parse_client_event(TimePoint time,
                                                 std::istringstream &iss) {
  std::string client_name;
  if (!(iss >> client_name)) {
    return Result<EventPtr, std::string>::err("Missing client name");
  }

  if (!Validator::is_valid_client_name(client_name)) {
    return Result<EventPtr, std::string>::err("Invalid client name: " +
                                              client_name);
  }

  return Result<EventPtr, std::string>::ok(
      std::make_unique<EventType>(time, std::move(client_name)));
}

template <typename EventType>
Result<EventPtr, std::string>
parse_client_table_event(TimePoint time, std::istringstream &iss) {
  std::string client_name;
  int table_number;
  if (!(iss >> client_name >> table_number)) {
    return Result<EventPtr, std::string>::err(
        "Missing client name or table number");
  }

  if (!Validator::is_valid_client_name(client_name)) {
    return Result<EventPtr, std::string>::err("Invalid client name: " +
                                              client_name);
  }

  if (!Validator::is_valid_table_number(table_number)) {
    return Result<EventPtr, std::string>::err("Invalid table number: " +
                                              std::to_string(table_number));
  }

  return Result<EventPtr, std::string>::ok(
      std::make_unique<EventType>(time, std::move(client_name), table_number));
}

using event_parser_func = std::function<Result<EventPtr, std::string>(
    TimePoint, std::istringstream &)>;

static const std::unordered_map<int, event_parser_func> event_parsers = {
    {1, parse_client_event<ClientArrivedEvent>},
    {2, parse_client_table_event<ClientSatEvent>},
    {3, parse_client_event<ClientWaitingEvent>},
    {4, parse_client_event<ClientLeftEvent>},
};

Result<EventPtr, std::string> Parser::parse_event(const std::string &line) {
  std::istringstream iss(line);
  std::string time_str;
  int event_id;

  if (!(iss >> time_str >> event_id)) {
    return Result<EventPtr, std::string>::err(line);
  }

  auto time_result = Parser::parse_time(time_str);
  if (time_result.is_err()) {
    return Result<EventPtr, std::string>::err(line);
  }

  TimePoint time = time_result.value();

  auto parser_it = event_parsers.find(event_id);
  if (parser_it == event_parsers.end()) {
    return Result<EventPtr, std::string>::err(line);
  }

  return parser_it->second(time, iss);
}
