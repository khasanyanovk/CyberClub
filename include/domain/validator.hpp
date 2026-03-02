#ifndef _VALIDATOR_HPP_
#define _VALIDATOR_HPP_

#include <string>

class Validator {
public:
  [[nodiscard]] static bool is_valid_client_name(const std::string &name);
  [[nodiscard]] static bool is_valid_table_number(int table_number);
  [[nodiscard]] static bool is_valid_table_number(int table_number,
                                                  int max_tables);
  [[nodiscard]] static bool is_valid_tables_count(int count);
  [[nodiscard]] static bool is_valid_cost(int cost);
};

#endif
