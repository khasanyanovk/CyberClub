#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include "event.hpp"
#include "result.hpp"
#include "time_point.hpp"
#include <string>

Result<TimePoint, std::string> parse_time(const std::string &time_str);

Result<EventPtr, std::string> parse_event(const std::string &line);

#endif
