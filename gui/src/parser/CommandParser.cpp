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

parser::TileUpdate parser::CommandParser::parseBct(const std::string& command) {
  int x, y;
  std::array<int, RESOURCE_COUNT> quantities = {0};

  int result = sscanf(command.c_str(), "bct %d %d %d %d %d %d %d %d",
                &x, &y, &quantities[0], &quantities[1],
                &quantities[2], &quantities[3],
                &quantities[4], &quantities[5]);
  if (result != 8)
    return TileUpdate(0, 0, quantities);
  return TileUpdate(x, y, quantities);
}
