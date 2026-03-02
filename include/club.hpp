#ifndef _CLUB_HPP_
#define _CLUB_HPP_

#include "event.hpp"
#include "parser.hpp"
#include "time_point.hpp"
#include <deque>
#include <map>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

struct table_stats {
  int total_minutes = 0;
  int revenue = 0;
};

struct table_state {
  std::string client_name;
  TimePoint occupied_since;
  bool is_occupied = false;
};

class Club {
private:
  club_config config_;
  std::vector<table_state> tables_;
  std::map<std::string, int>
      clients_; // client_name -> table_number (0 if waiting)
  std::deque<std::string> waiting_queue_;
  std::vector<table_stats> stats_;
  TimePoint current_time_;

  [[nodiscard]] bool is_open(TimePoint time) const;
  [[nodiscard]] bool has_free_tables() const;
  [[nodiscard]] std::optional<int> find_free_table() const;
  void free_table(int table_number, TimePoint time);
  void occupy_table(int table_number, const std::string &client_name,
                    TimePoint time);
  [[nodiscard]] int calculate_table_cost(int minutes) const;

  // Event handlers
  using event_handler = void (Club::*)(const Event *, std::vector<EventPtr> &);

  void handle_client_arrived(const Event *event, std::vector<EventPtr> &output);
  void handle_client_sat(const Event *event, std::vector<EventPtr> &output);
  void handle_client_waiting(const Event *event, std::vector<EventPtr> &output);
  void handle_client_left(const Event *event, std::vector<EventPtr> &output);

  static const std::unordered_map<int, event_handler> handlers;

public:
  explicit Club(club_config config);

  [[nodiscard]] std::vector<EventPtr> process_event(EventPtr event);
  [[nodiscard]] std::vector<EventPtr> close_club();
  [[nodiscard]] const std::vector<table_stats> &get_stats() const {
    return stats_;
  }
  [[nodiscard]] TimePoint open_time() const { return config_.open_time; }
  [[nodiscard]] TimePoint close_time() const { return config_.close_time; }
};

#endif
