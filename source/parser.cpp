#include "parser.hpp"
#include "events.hpp"
#include <functional>
#include <regex>
#include <sstream>
#include <unordered_map>

Result<TimePoint, std::string> parse_time(const std::string &time_str) {
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

template <typename EventType>
Result<EventPtr, std::string> parse_client_event(TimePoint time,
                                                 std::istringstream &iss) {
  std::string client_name;
  if (!(iss >> client_name)) {
    return Result<EventPtr, std::string>::err("Missing client name");
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

Result<EventPtr, std::string> parse_event(const std::string &line) {
  std::istringstream iss(line);
  std::string time_str;
  int event_id;

  if (!(iss >> time_str >> event_id)) {
    return Result<EventPtr, std::string>::err(line);
  }

  auto time_result = parse_time(time_str);
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
