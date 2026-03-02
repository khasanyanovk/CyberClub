#include "domain/validator.hpp"

bool Validator::is_valid_client_name(const std::string &name) {
  if (name.empty()) {
    return false;
  }

  for (char c : name) {
    if (!((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_' ||
          c == '-')) {
      return false;
    }
  }

  return true;
}

bool Validator::is_valid_table_number(int table_number) {
  return table_number > 0;
}

bool Validator::is_valid_table_number(int table_number, int max_tables) {
  return table_number > 0 && table_number <= max_tables;
}

bool Validator::is_valid_tables_count(int count) { return count > 0; }

bool Validator::is_valid_cost(int cost) { return cost > 0; }
