#include "CommandParser.hpp"
#include <cstdio>
#include <sstream>

/**
 * @brief Parses the 'msz' command to extract map size.
 * @param command The command string (e.g., "msz 10 20").
 * @return MapSize structure with width and height, or (0,0) if parsing fails.
 */
parser::MapSize parser::CommandParser::parseMsz(const std::string &command) {
  int width, height;
  int result = sscanf(command.c_str(), "msz %d %d", &width, &height);
  if (result != 2)
    return MapSize(0, 0);
  return MapSize(width, height);
}

/**
 * @brief Parses the 'sgt' command to extract the time unit.
 * @param command The command string (e.g., "sgt 100").
 * @return TimeUnit structure with the time value, or 0 if parsing fails.
 */
parser::TimeUnit parser::CommandParser::parseSgt(const std::string &command) {
  int time;
  int result = sscanf(command.c_str(), "sgt %d", &time);
  if (result != 1)
    return TimeUnit(0);
  return TimeUnit(time);
}

/**
 * @brief Parses the 'tna' command to extract team names.
 * @param command The command string (e.g., "tna team1 team2").
 * @return TeamNames structure with a vector of team names.
 */
parser::TeamNames parser::CommandParser::parseTna(const std::string &command) {
  std::vector<std::string> names;
  std::istringstream iss(command);
  std::string token;

  if (!(iss >> token) || token != "tna")
    return TeamNames(names);

  while (iss >> token)
    names.push_back(token);
  return TeamNames(names);
}

/**
 * @brief Parses the 'bct' command to extract tile resource information.
 * @param command The command string (e.g., "bct 1 2 0 1 2 3 4 5 6").
 * @return TileUpdate structure with tile coordinates and resource counts.
 * @throws std::runtime_error if the command format is invalid.
 */
parser::TileUpdate parser::CommandParser::parseBct(const std::string &command) {
  int x, y;
  std::array<int, RESOURCE_COUNT> resources = {0};

  int result =
      sscanf(command.c_str(), "bct %d %d %d %d %d %d %d %d %d", &x, &y,
             &resources[0], &resources[1], &resources[2], &resources[3],
             &resources[4], &resources[5], &resources[6]);
  if (result != 9)
    throw std::runtime_error("Invalid bct command format");
  return TileUpdate(x, y, resources);
}

/**
 * @brief Parses the 'pnw' command to extract player information.
 * @param command The command string (e.g., "pnw #1 2 3 4 5 team").
 * @return PlayerInfo structure with player details.
 * @throws std::runtime_error if the command format is invalid.
 */
parser::PlayerInfo parser::CommandParser::parsePnw(const std::string &command) {
  int id, x, y, level;
  int orientationInt;
  char teamName[256] = {0};

  int result = std::sscanf(command.c_str(), "pnw #%d %d %d %d %d %255s", &id,
                           &x, &y, &orientationInt, &level, teamName);

  if (result != 6) {
    throw std::runtime_error("Failed to parse pnw command");
  }
  gui::Orientation orientation = static_cast<gui::Orientation>(orientationInt);
  return PlayerInfo(id, x, y, orientation, level, std::string(teamName));
}

/**
 * @brief Parses the 'ppo' command to extract player position update.
 * @param command The command string (e.g., "ppo #1 2 3 4").
 * @return PlayerPositionUpdate structure with player position and orientation.
 * @throws std::runtime_error if the command format or orientation is invalid.
 */
parser::PlayerPositionUpdate parser::CommandParser::parsePpo(
    const std::string &command) {
  int id, x, y;
  int orientationInt;

  int result = std::sscanf(command.c_str(), "ppo #%d %d %d %d", &id, &x, &y,
                           &orientationInt);
  if (result != 4)
    throw std::runtime_error("Invalid ppo command format");
  if (orientationInt < 1 || orientationInt > 4)
    throw std::runtime_error("Invalid orientation value in ppo command");
  gui::Orientation orientation = static_cast<gui::Orientation>(orientationInt);
  return PlayerPositionUpdate(id, x, y, orientation);
}

/**
 * @brief Parses the 'plv' command to extract player level update.
 * @param command The command string (e.g., "plv #1 2").
 * @return PlayerLevelUpdate structure with player ID and level.
 * @throws std::runtime_error if the command format is invalid.
 */
parser::PlayerLevelUpdate parser::CommandParser::parsePlv(
    const std::string &command) {
  int id, level;

  int result = std::sscanf(command.c_str(), "plv #%d %d", &id, &level);

  if (result != 2)
    throw std::runtime_error("Invalid plv command format");
  return PlayerLevelUpdate(id, level);
}

/**
 * @brief Parses the 'pin' command to extract player inventory.
 * @param command The command string (e.g., "pin #1 2 3 0 1 2 3 4 5 6").
 * @return PlayerInventory structure with player ID, position, and resources.
 * @throws std::runtime_error if the command format is invalid.
 */
parser::PlayerInventory parser::CommandParser::parsePin(
    const std::string &command) {
  int id, x, y;
  std::array<int, RESOURCE_COUNT> resources = {0};

  int result =
      std::sscanf(command.c_str(), "pin #%d %d %d %d %d %d %d %d %d %d", &id,
                  &x, &y, &resources[0], &resources[1], &resources[2],
                  &resources[3], &resources[4], &resources[5], &resources[6]);

  if (result != 10)
    throw std::runtime_error("Invalid pin command format");
  return PlayerInventory(id, x, y, resources);
}

/**
 * @brief Parses the 'enw' command to extract egg laid event.
 * @param command The command string (e.g., "enw #1 #2 3 4").
 * @return EggLaid structure with egg and player IDs and position.
 * @throws std::runtime_error if the command format is invalid.
 */
parser::EggLaid parser::CommandParser::parseEnw(const std::string &command) {
  int idEgg, idPlayer, x, y;

  int result = std::sscanf(command.c_str(), "enw #%d #%d %d %d", &idEgg,
                           &idPlayer, &x, &y);

  if (result != 4)
    throw std::runtime_error("Invalid enw command format");
  return EggLaid(idEgg, idPlayer, x, y);
}

/**
 * @brief Parses the 'ebo' command to extract egg hatch event.
 * @param command The command string (e.g., "ebo #1").
 * @return EggHatch structure with egg ID.
 * @throws std::runtime_error if the command format is invalid.
 */
parser::EggHatch parser::CommandParser::parseEbo(const std::string &command) {
  int id;
  int result = std::sscanf(command.c_str(), "ebo #%d", &id);

  if (result != 1)
    throw std::runtime_error("Invalid ebo command format");
  return EggHatch(id);
}

/**
 * @brief Parses the 'edi' command to extract egg death event.
 * @param command The command string (e.g., "edi #1").
 * @return EggDeath structure with egg ID.
 * @throws std::runtime_error if the command format is invalid.
 */
parser::EggDeath parser::CommandParser::parseEdi(const std::string &command) {
  int id;
  int result = std::sscanf(command.c_str(), "edi #%d", &id);

  if (result != 1)
    throw std::runtime_error("Invalid edi command format");
  return EggDeath(id);
}

/**
 * @brief Parses the 'pdi' command to extract player death event.
 * @param command The command string (e.g., "pdi #1").
 * @return PlayerDeath structure with player ID.
 * @throws std::runtime_error if the command format is invalid.
 */
parser::PlayerDeath parser::CommandParser::parsePdi(
    const std::string &command) {
  int id;
  int result = std::sscanf(command.c_str(), "pdi #%d", &id);

  if (result != 1)
    throw std::runtime_error("Invalid pdi command format");
  return PlayerDeath(id);
}

/**
 * @brief Parses the 'pic' command to extract incantation start event.
 * @param command The command string (e.g., "pic 1 2 3 #4 #5").
 * @return Incantation structure with coordinates, level, and player IDs.
 * @throws std::runtime_error if the command format is invalid.
 */
parser::Incantation parser::CommandParser::parsePic(const std::string &command) {
  int x, y, level;
  std::vector<int> playersNumber;

  int result = std::sscanf(command.c_str(), "pic %d %d %d", &x, &y, &level);
  if (result != 3) {
    throw std::runtime_error("Invalid pic command format");
  }

  std::istringstream iss(command);
  std::string token;

  iss >> token;
  iss >> token;
  iss >> token;
  iss >> token;

  while (iss >> token) {
    if (token[0] == '#') {
      int playerId = std::stoi(token.substr(1));
      playersNumber.push_back(playerId);
    }
  }

  return Incantation(x, y, level, playersNumber);
}

/**
 * @brief Parses the 'pie' command to extract incantation end event.
 * @param command The command string (e.g., "pie 1 2 1").
 * @return IncantationEnd structure with coordinates and success flag.
 * @throws std::runtime_error if the command format is invalid.
 */
parser::IncantationEnd parser::CommandParser::parsePie(
    const std::string &command) {
  int x, y, result;
  int parsed = std::sscanf(command.c_str(), "pie %d %d %d", &x, &y, &result);
  if (parsed != 3)
    throw std::runtime_error("Invalid pie command format");

  return IncantationEnd(x, y, result == 1);
}

/**
 * @brief Parses the 'pfk' command to extract fork event.
 * @param command The command string (e.g., "pfk #1").
 * @return ForkEvent structure with player ID.
 * @throws std::runtime_error if the command format is invalid.
 */
parser::ForkEvent parser::CommandParser::parsePfk(const std::string &command) {
  int playerId;
  int result = std::sscanf(command.c_str(), "pfk #%d", &playerId);

  if (result != 1)
    throw std::runtime_error("Invalid pfk command format");
  return ForkEvent(playerId);
}

/**
 * @brief Parses the 'pdr' command to extract drop resource event.
 * @param command The command string (e.g., "pdr #1 2").
 * @return DropResource structure with player ID and resource number.
 * @throws std::runtime_error if the command format or resource number is invalid.
 */
parser::DropResource parser::CommandParser::parsePdr(
    const std::string &command) {
  int playerId, resourceNumber;
  int result =
      std::sscanf(command.c_str(), "pdr #%d %d", &playerId, &resourceNumber);

  if (result != 2)
    throw std::runtime_error("Invalid pdr command format");
  if (resourceNumber < 0 || resourceNumber >= static_cast<int>(RESOURCE_COUNT))
    throw std::runtime_error("Invalid resource number in pdr command");
  return DropResource(playerId, resourceNumber);
}

/**
 * @brief Parses the 'pgt' command to extract collect resource event.
 * @param command The command string (e.g., "pgt #1 2").
 * @return CollectResource structure with player ID and resource number.
 * @throws std::runtime_error if the command format or resource number is invalid.
 */
parser::CollectResource parser::CommandParser::parsePgt(
    const std::string &command) {
  int playerId, resourceNumber;
  int result =
      std::sscanf(command.c_str(), "pgt #%d %d", &playerId, &resourceNumber);

  if (result != 2)
    throw std::runtime_error("Invalid pgt command format");
  if (resourceNumber < 0 || resourceNumber >= static_cast<int>(RESOURCE_COUNT))
    throw std::runtime_error("Invalid resource number in pgt command");
  return CollectResource(playerId, resourceNumber);
}

/**
 * @brief Parses the 'pex' command to extract player expulsion event.
 * @param command The command string (e.g., "pex #1").
 * @return PlayerExpulsion structure with player ID.
 * @throws std::runtime_error if the command format is invalid.
 */
parser::PlayerExpulsion parser::CommandParser::parsePex(
    const std::string &command) {
  int playerId;
  int result = std::sscanf(command.c_str(), "pex #%d", &playerId);

  if (result != 1)
    throw std::runtime_error("Invalid pex command format");
  return PlayerExpulsion(playerId);
}

/**
 * @brief Parses the 'pbc' command to extract broadcast event.
 * @param command The command string (e.g., "pbc #1 message").
 * @return BroadcastEvent structure with player ID and message.
 * @throws std::runtime_error if the command format or player ID is invalid.
 */
parser::BroadcastEvent parser::CommandParser::parsePbc(
    const std::string &command) {
  std::istringstream iss(command);
  std::string prefix;
  std::string playerIdToken;
  std::string message;
  int playerId;

  if (!(iss >> prefix >> playerIdToken) || prefix != "pbc")
    throw std::runtime_error("Invalid pbc command format");

  if (playerIdToken[0] != '#')
    throw std::runtime_error("Invalid player ID format (missing '#')");
  try {
    playerId = std::stoi(playerIdToken.substr(1));
  } catch (...) {
    throw std::runtime_error("Invalid player ID number");
  }
  std::getline(iss, message);
  if (!message.empty() && message[0] == ' ')
    message.erase(0, 1);
  return BroadcastEvent(playerId, message);
}

/**
 * @brief Parses the 'smg' command to extract server message event.
 * @param command The command string (e.g., "smg message").
 * @return ServerMessageEvent structure with the message.
 * @throws std::runtime_error if the command format is invalid.
 */
parser::ServerMessageEvent parser::CommandParser::parseSmg(
    const std::string &command) {
  if (command.size() < 4)
    throw std::runtime_error("Invalid smg command format");
  std::string message = command.substr(4);
  return ServerMessageEvent(message);
}

/**
 * @brief Parses the 'seg' command to extract game over event.
 * @param command The command string (e.g., "seg teamName").
 * @return GameOverEvent structure with the winning team name.
 * @throws std::runtime_error if the command format or team name is invalid.
 */
parser::GameOverEvent parser::CommandParser::parseSeg(
    const std::string &command) {
  std::istringstream iss(command);
  std::string prefix;
  std::string teamName;

  if (!(iss >> prefix >> teamName) || prefix != "seg")
    throw std::runtime_error("Invalid seg command format");
  if (teamName.empty())
    throw std::runtime_error(
        "Winning team name cannot be empty in seg command");
  return GameOverEvent(teamName);
}
