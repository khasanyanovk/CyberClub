#ifndef _CLUB_MANAGER_HPP_
#define _CLUB_MANAGER_HPP_

#include "ireader.hpp"
#include "iwriter.hpp"
#include "result.hpp"
#include <memory>
#include <string>

class ClubManager {
private:
  std::unique_ptr<IReader> reader_;
  std::unique_ptr<IWriter> writer_;

  void write_output(const std::string &line);

public:
  ClubManager(std::unique_ptr<IReader> reader, std::unique_ptr<IWriter> writer);

  [[nodiscard]] Result<void, std::string> run();
};

#endif
