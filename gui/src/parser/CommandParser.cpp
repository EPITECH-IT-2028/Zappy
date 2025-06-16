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
  std::array<int, RESOURCE_COUNT> resources = {0};

  int result = sscanf(command.c_str(), "bct %d %d %d %d %d %d %d %d %d",
                &x, &y, &resources[0], &resources[1],
                &resources[2], &resources[3],
                &resources[4], &resources[5], &resources[6]);
  if (result != 9)
    throw std::runtime_error("Invalid bct command format");
  return TileUpdate(x, y, resources);
}

parser::PlayerInfo parser::CommandParser::parsePnw(const std::string& command) {
    int id, x, y, orientation, level;
    char teamName[256];

    int result = std::sscanf(command.c_str(), "pnw %d %d %d %d %d %255s",
                 &id, &x, &y, &orientation, &level, teamName);
    if (result != 6)
      throw std::runtime_error("Invalid pnw command format");
    return PlayerInfo(id, x, y, orientation, level, std::string(teamName));
}

parser::PlayerPositionUpdate parser::CommandParser::parsePpo(const std::string& command) {
    int id, x, y, orientation;

    int result = std::sscanf(command.c_str(), "ppo %d %d %d %d",
                 &id, &x, &y, &orientation);
    if (result != 4)
      throw std::runtime_error("Invalid ppo command format");
    return PlayerPositionUpdate(id, x, y, orientation);
}

parser::PlayerLevelUpdate parser::CommandParser::parsePlv(const std::string &command) {
  int id, level;

  int result = std::sscanf(command.c_str(), "plv %d %d", &id, &level);

  if (result != 2)
    throw std::runtime_error("Invalid plv command format");
  return PlayerLevelUpdate(id, level);
}

parser::PlayerInventory parser::CommandParser::parsePin(const std::string &command) {
  int id, x, y;
  std::array<int, RESOURCE_COUNT> resources = {0};

  int result = std::sscanf(command.c_str(), "pin %d %d %d %d %d %d %d %d %d %d",
               &id, &x, &y, &resources[0], &resources[1],
               &resources[2], &resources[3],
               &resources[4], &resources[5], &resources[6]);

  if (result != 10)
    throw std::runtime_error("Invalid pin command format");
  return PlayerInventory(id, x, y, resources);
}

parser::EggLaid parser::CommandParser::parseEnw(const std::string &command) {
  int idEgg, idPlayer, x, y;

  int result = std::sscanf(command.c_str(), "enw %d %d %d %d", &idEgg, &idPlayer, &x, &y);

  if (result != 4)
    throw std::runtime_error("Invalid enw command format");
  return EggLaid(idEgg, idPlayer, x, y);
}

parser::EggHatch parser::CommandParser::parseEbo(const std::string &command) {
  int id;
  int result = std::sscanf(command.c_str(), "ebo %d", &id);

  if (result != 1)
    throw std::runtime_error("Invalid ebo command format");
  return EggHatch(id);
}

parser::EggDeath parser::CommandParser::parseEdi(const std::string &command) {
  int id;
  int result = std::sscanf(command.c_str(), "edi %d", &id);

  if (result != 1)
    throw std::runtime_error("Invalid edi command format");
  return EggDeath(id);
}

parser::PlayerDeath parser::CommandParser::parsePdi(const std::string &command) {
  int id;
  int result = std::sscanf(command.c_str(), "pdi %d", &id);

  if (result != 1)
    throw std::runtime_error("Invalid pdi command format");
  return PlayerDeath(id);
}
