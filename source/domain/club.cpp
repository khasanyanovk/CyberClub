#include "domain/club.hpp"
#include "events/events.hpp"
#include <algorithm>

Club::Club(club_config config)
    : config_(config), tables_(config.tables_count),
      stats_(config.tables_count), current_time_(config.open_time) {}

bool Club::is_open(TimePoint time) const {
  return time >= config_.open_time && time < config_.close_time;
}

bool Club::has_free_tables() const {
  return std::any_of(tables_.begin(), tables_.end(),
                     [](const table_state &t) { return !t.is_occupied; });
}

std::optional<int> Club::find_free_table() const {
  for (size_t i = 0; i < tables_.size(); ++i) {
    if (!tables_[i].is_occupied) {
      return static_cast<int>(i + 1);
    }
  }
  return std::nullopt;
}

void Club::free_table(int table_number, TimePoint time) {
  int idx = table_number - 1;
  if (idx < 0 || idx >= static_cast<int>(tables_.size())) {
    return;
  }

  if (tables_[idx].is_occupied) {
    int minutes_occupied =
        static_cast<int>(time.to_minutes().count() -
                         tables_[idx].occupied_since.to_minutes().count());
    stats_[idx].total_minutes += minutes_occupied;
    stats_[idx].revenue += calculate_table_cost(minutes_occupied);

    tables_[idx].is_occupied = false;
    tables_[idx].client_name.clear();
  }
}

void Club::occupy_table(int table_number, const std::string &client_name,
                        TimePoint time) {
  int idx = table_number - 1;
  if (idx < 0 || idx >= static_cast<int>(tables_.size())) {
    return;
  }

  tables_[idx].is_occupied = true;
  tables_[idx].client_name = client_name;
  tables_[idx].occupied_since = time;
  clients_[client_name] = table_number;
}

int Club::calculate_table_cost(int minutes) const {
  if (minutes <= 0) {
    return 0;
  }
  int hours = (minutes + 59) / 60;
  return hours * config_.cost_per_hour;
}

const std::unordered_map<int, Club::event_handler> Club::handlers = {
    {1, &Club::handle_client_arrived},
    {2, &Club::handle_client_sat},
    {3, &Club::handle_client_waiting},
    {4, &Club::handle_client_left}};

std::vector<EventPtr> Club::process_event(EventPtr event) {
  std::vector<EventPtr> output;
  current_time_ = event->time();

  output.push_back(std::move(event));
  const Event *ev = output.back().get();

  auto it = handlers.find(ev->id());
  if (it != handlers.end()) {
    (this->*(it->second))(ev, output);
  }

  return output;
}

void Club::handle_client_arrived(const Event *event,
                                 std::vector<EventPtr> &output) {
  const auto *arrived = dynamic_cast<const ClientArrivedEvent *>(event);
  if (!arrived)
    return;

  const std::string &name = arrived->client_name();

  if (!is_open(arrived->time())) {
    output.push_back(
        std::make_unique<ErrorEvent>(arrived->time(), "NotOpenYet"));
    return;
  }

  if (clients_.contains(name)) {
    output.push_back(
        std::make_unique<ErrorEvent>(arrived->time(), "YouShallNotPass"));
    return;
  }

  clients_[name] = 0;
}

void Club::handle_client_sat(const Event *event,
                             std::vector<EventPtr> &output) {
  const auto *sat = dynamic_cast<const ClientSatEvent *>(event);
  if (!sat)
    return;

  const std::string &name = sat->client_name();
  int table_num = sat->table_number();

  if (!clients_.contains(name)) {
    output.push_back(
        std::make_unique<ErrorEvent>(sat->time(), "ClientUnknown"));
    return;
  }

  int idx = table_num - 1;
  if (idx < 0 || idx >= static_cast<int>(tables_.size()) ||
      tables_[idx].is_occupied) {
    output.push_back(std::make_unique<ErrorEvent>(sat->time(), "PlaceIsBusy"));
    return;
  }

  int prev_table = clients_[name];
  if (prev_table > 0) {
    free_table(prev_table, sat->time());
  }

  occupy_table(table_num, name, sat->time());

  auto it = std::find(waiting_queue_.begin(), waiting_queue_.end(), name);
  if (it != waiting_queue_.end()) {
    waiting_queue_.erase(it);
  }
}

void Club::handle_client_waiting(const Event *event,
                                 std::vector<EventPtr> &output) {
  const auto *waiting = dynamic_cast<const ClientWaitingEvent *>(event);
  if (!waiting)
    return;

  const std::string &name = waiting->client_name();

  if (!clients_.contains(name)) {
    output.push_back(
        std::make_unique<ErrorEvent>(waiting->time(), "ClientUnknown"));
    return;
  }

  if (has_free_tables()) {
    output.push_back(
        std::make_unique<ErrorEvent>(waiting->time(), "ICanWaitNoLonger!"));
    return;
  }

  if (static_cast<int>(waiting_queue_.size()) >= config_.tables_count) {
    clients_.erase(name);
    output.push_back(
        std::make_unique<ClientKickedEvent>(waiting->time(), name));
    return;
  }

  waiting_queue_.push_back(name);
}

void Club::handle_client_left(const Event *event,
                              std::vector<EventPtr> &output) {
  const auto *left = dynamic_cast<const ClientLeftEvent *>(event);
  if (!left)
    return;

  const std::string &name = left->client_name();

  if (!clients_.contains(name)) {
    output.push_back(
        std::make_unique<ErrorEvent>(left->time(), "ClientUnknown"));
    return;
  }

  int prev_table = clients_[name];
  if (prev_table > 0) {
    free_table(prev_table, left->time());
  }

  clients_.erase(name);

  auto it = std::find(waiting_queue_.begin(), waiting_queue_.end(), name);
  if (it != waiting_queue_.end()) {
    waiting_queue_.erase(it);
  }

  if (prev_table > 0 && !waiting_queue_.empty()) {
    std::string next_client = waiting_queue_.front();
    waiting_queue_.pop_front();

    occupy_table(prev_table, next_client, left->time());
    output.push_back(std::make_unique<ClientSeatedEvent>(
        left->time(), next_client, prev_table));
  }
}

std::vector<EventPtr> Club::close_club() {
  std::vector<EventPtr> output;

  for (const auto &[name, table_num] : clients_) {
    if (table_num > 0) {
      free_table(table_num, config_.close_time);
    }
    output.push_back(
        std::make_unique<ClientKickedEvent>(config_.close_time, name));
  }

  clients_.clear();
  waiting_queue_.clear();

  return output;
}
