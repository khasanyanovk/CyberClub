#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include "event.hpp"
#include "result.hpp"
#include "time_point.hpp"
#include <string>
#include <vector>

struct club_config {
  int tables_count;
  TimePoint open_time, close_time;
  int cost_per_hour;
};

class Parser {

public:
  [[nodiscard]] static Result<TimePoint, std::string>
  parse_time(const std::string &time_str);

  [[nodiscard]] static Result<club_config, std::string>
  parse_config(const std::vector<std::string> &lines);

  [[nodiscard]] static Result<EventPtr, std::string>
  parse_event(const std::string &line);
};

#endif
