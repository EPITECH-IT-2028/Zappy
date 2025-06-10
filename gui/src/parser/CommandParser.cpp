#include "CommandParser.hpp"
#include <sstream>

parser::MapSize parser::CommandParser::parseMsz(const std::string& command) {
  int width, height;
  sscanf(command.c_str(), "msz %d %d", &width, &height);
  return MapSize(width, height);
}

parser::TimeUnit parser::CommandParser::parseSgt(const std::string& command) {
  int time;
  sscanf(command.c_str(), "sgt %d", &time);
  return TimeUnit(time);
}

parser::TeamNames parser::CommandParser::parseTna(const std::string& command) {
  std::vector<std::string> names;
  std::istringstream iss(command);
  std::string token;

  iss >> token;
  while (iss >> token)
    names.push_back(token);
  return TeamNames(names);
}
