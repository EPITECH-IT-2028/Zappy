#include "CommandParser.hpp"
#include <cstdio>
#include <sstream>

parser::MapSize parser::CommandParser::parseMsz(const std::string& command) {
  int width, height;
  int result = sscanf(command.c_str(), "msz %d %d", &width, &height);
  if (result != 2)
    return MapSize(0, 0);
  return MapSize(width, height);
}

parser::TimeUnit parser::CommandParser::parseSgt(const std::string& command) {
  int time;
  int result = sscanf(command.c_str(), "sgt %d", &time);
  if (result != 1)
    return TimeUnit(0);
  return TimeUnit(time);
}

parser::TeamNames parser::CommandParser::parseTna(const std::string& command) {
  std::vector<std::string> names;
  std::istringstream iss(command);
  std::string token;

  if (!(iss >> token) || token != "tna")
    return TeamNames(names);

  while (iss >> token)
    names.push_back(token);
  return TeamNames(names);
}
