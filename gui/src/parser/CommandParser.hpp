#pragma once

#include <array>
#include <string>
#include <vector>
#include "entities/Orientation.hpp"

namespace parser {
  inline constexpr std::size_t RESOURCE_COUNT = 7;

  /**
   * @brief Structure representing the map size.
   */
  struct MapSize {
      int width;  ///< Map width
      int height; ///< Map height
      MapSize(int w, int h) : width(w), height(h) {}
  };

  /**
   * @brief Structure representing the time unit.
   */
  struct TimeUnit {
      int time; ///< Time unit value
      TimeUnit(int t) : time(t) {}
  };

  /**
   * @brief Structure representing a list of team names.
   */
  struct TeamNames {
      std::vector<std::string> names; ///< Vector of team names
      TeamNames(const std::vector<std::string>& n) : names(n) {}
  };

  /**
   * @brief Structure representing a tile update with resources.
   */
  struct TileUpdate {
      int x; ///< Tile X coordinate
      int y; ///< Tile Y coordinate
      std::array<int, RESOURCE_COUNT> resources; ///< Resource counts on the tile
      TileUpdate(int x, int y, const std::array<int, RESOURCE_COUNT>& q)
          : x(x), y(y), resources(q) {}
  };

  /**
   * @brief Structure representing player information.
   */
  struct PlayerInfo {
      int id; ///< Player ID
      int x; ///< Player X coordinate
      int y; ///< Player Y coordinate
      gui::Orientation orientation; ///< Player orientation
      int level; ///< Player level
      std::string teamName; ///< Player's team name
      PlayerInfo(int id, int x, int y, gui::Orientation orientation, int level,
                 const std::string& team)
          : id(id), x(x), y(y), orientation(orientation), level(level), teamName(team) {}
  };

  /**
   * @brief Structure representing a player position update.
   */
  struct PlayerPositionUpdate {
      int id; ///< Player ID
      int x; ///< Player X coordinate
      int y; ///< Player Y coordinate
      gui::Orientation orientation; ///< Player orientation
      PlayerPositionUpdate(int id, int x, int y, gui::Orientation orientation)
          : id(id), x(x), y(y), orientation(orientation) {}
  };

  /**
   * @brief Structure representing a player level update.
   */
  struct PlayerLevelUpdate {
      int id; ///< Player ID
      int level; ///< Player level
      PlayerLevelUpdate(int id, int level) : id(id), level(level) {}
  };

  /**
   * @brief Structure representing a player's inventory.
   */
  struct PlayerInventory {
      int id; ///< Player ID
      int x; ///< Player X coordinate
      int y; ///< Player Y coordinate
      std::array<int, RESOURCE_COUNT> resources; ///< Resource counts
      PlayerInventory(int id, int x, int y,
                      const std::array<int, RESOURCE_COUNT>& res)
          : id(id), x(x), y(y), resources(res) {}
  };

  /**
   * @brief Structure representing an egg laid event.
   */
  struct EggLaid {
      int idEgg; ///< Egg ID
      int idPlayer; ///< Player ID who laid the egg
      int x; ///< Egg X coordinate
      int y; ///< Egg Y coordinate
      EggLaid(int idEgg, int idPlayer, int x, int y)
          : idEgg(idEgg), idPlayer(idPlayer), x(x), y(y) {}
  };

  /**
   * @brief Structure representing an egg hatch event.
   */
  struct EggHatch {
      int eggId; ///< Egg ID
      EggHatch(int id) : eggId(id) {}
  };

  /**
   * @brief Structure representing an egg death event.
   */
  struct EggDeath {
      int eggId; ///< Egg ID
      EggDeath(int id) : eggId(id) {}
  };

  /**
   * @brief Structure representing a player death event.
   */
  struct PlayerDeath {
      int id; ///< Player ID
      PlayerDeath(int id) : id(id) {}
  };

  /**
   * @brief Structure representing an incantation start event.
   */
  struct Incantation {
      int x; ///< X coordinate
      int y; ///< Y coordinate
      int level; ///< Incantation level
      std::vector<int> playersNumber; ///< IDs of players involved
      Incantation(int x, int y, int level, const std::vector<int>& players)
          : x(x), y(y), level(level), playersNumber(players) {}
  };

  /**
   * @brief Structure representing an incantation end event.
   */
  struct IncantationEnd {
      int x; ///< X coordinate
      int y; ///< Y coordinate
      bool success; ///< Whether the incantation was successful
      IncantationEnd(int x, int y, bool success)
          : x(x), y(y), success(success) {}
  };

  /**
   * @brief Structure representing a fork event.
   */
  struct ForkEvent {
      int playerID; ///< Player ID
      ForkEvent(int playerID) : playerID(playerID) {}
  };

  /**
   * @brief Structure representing a drop resource event.
   */
  struct DropResource {
      int playerID; ///< Player ID
      int resourceNumber; ///< Resource number
      DropResource(int playerID, int resourceNumber)
          : playerID(playerID), resourceNumber(resourceNumber) {}
  };

  /**
   * @brief Structure representing a collect resource event.
   */
  struct CollectResource {
      int playerID; ///< Player ID
      int resourceNumber; ///< Resource number
      CollectResource(int playerID, int resourceNumber)
          : playerID(playerID), resourceNumber(resourceNumber) {}
  };

  /**
   * @brief Structure representing a player expulsion event.
   */
  struct PlayerExpulsion {
      int playerID; ///< Player ID
      PlayerExpulsion(int playerID) : playerID(playerID) {}
  };

  /**
   * @brief Structure representing a broadcast event.
   */
  struct BroadcastEvent {
      int playerID; ///< Player ID
      std::string message; ///< Broadcast message
      BroadcastEvent(int id, const std::string& msg)
          : playerID(id), message(msg) {}
  };

  /**
   * @brief Structure representing a server message event.
   */
  struct ServerMessageEvent {
      std::string message; ///< Server message
      ServerMessageEvent(const std::string& msg) : message(msg) {}
  };

  /**
   * @brief Structure representing a game over event.
   */
  struct GameOverEvent {
      std::string winningTeamName; ///< Name of the winning team
      GameOverEvent(const std::string& name) : winningTeamName(name) {}
  };

  /**
   * @brief CommandParser class for parsing server commands into structured data.
   */
  class CommandParser {
    public:
      CommandParser() = default;
      ~CommandParser() = default;

      /**
       * @brief Parses the 'msz' command to extract map size.
       * @param command The command string.
       * @return MapSize structure.
       */
      static MapSize parseMsz(const std::string& command);
      /**
       * @brief Parses the 'sgt' command to extract the time unit.
       * @param command The command string.
       * @return TimeUnit structure.
       */
      static TimeUnit parseSgt(const std::string& command);
      /**
       * @brief Parses the 'tna' command to extract team names.
       * @param command The command string.
       * @return TeamNames structure.
       */
      static TeamNames parseTna(const std::string& command);
      /**
       * @brief Parses the 'bct' command to extract tile resource information.
       * @param command The command string.
       * @return TileUpdate structure.
       */
      static TileUpdate parseBct(const std::string& command);
      /**
       * @brief Parses the 'pnw' command to extract player information.
       * @param command The command string.
       * @return PlayerInfo structure.
       */
      static PlayerInfo parsePnw(const std::string& command);
      /**
       * @brief Parses the 'ppo' command to extract player position update.
       * @param command The command string.
       * @return PlayerPositionUpdate structure.
       */
      static PlayerPositionUpdate parsePpo(const std::string& command);
      /**
       * @brief Parses the 'plv' command to extract player level update.
       * @param command The command string.
       * @return PlayerLevelUpdate structure.
       */
      static PlayerLevelUpdate parsePlv(const std::string& command);
      /**
       * @brief Parses the 'pin' command to extract player inventory.
       * @param command The command string.
       * @return PlayerInventory structure.
       */
      static PlayerInventory parsePin(const std::string& command);
      /**
       * @brief Parses the 'enw' command to extract egg laid event.
       * @param command The command string.
       * @return EggLaid structure.
       */
      static EggLaid parseEnw(const std::string& command);
      /**
       * @brief Parses the 'ebo' command to extract egg hatch event.
       * @param command The command string.
       * @return EggHatch structure.
       */
      static EggHatch parseEbo(const std::string& command);
      /**
       * @brief Parses the 'edi' command to extract egg death event.
       * @param command The command string.
       * @return EggDeath structure.
       */
      static EggDeath parseEdi(const std::string& command);
      /**
       * @brief Parses the 'pdi' command to extract player death event.
       * @param command The command string.
       * @return PlayerDeath structure.
       */
      static PlayerDeath parsePdi(const std::string& command);
      /**
       * @brief Parses the 'pic' command to extract incantation start event.
       * @param command The command string.
       * @return Incantation structure.
       */
      static Incantation parsePic(const std::string& command);
      /**
       * @brief Parses the 'pie' command to extract incantation end event.
       * @param command The command string.
       * @return IncantationEnd structure.
       */
      static IncantationEnd parsePie(const std::string& command);
      /**
       * @brief Parses the 'pfk' command to extract fork event.
       * @param command The command string.
       * @return ForkEvent structure.
       */
      static ForkEvent parsePfk(const std::string& command);
      /**
       * @brief Parses the 'pdr' command to extract drop resource event.
       * @param command The command string.
       * @return DropResource structure.
       */
      static DropResource parsePdr(const std::string& command);
      /**
       * @brief Parses the 'pgt' command to extract collect resource event.
       * @param command The command string.
       * @return CollectResource structure.
       */
      static CollectResource parsePgt(const std::string& command);
      /**
       * @brief Parses the 'pex' command to extract player expulsion event.
       * @param command The command string.
       * @return PlayerExpulsion structure.
       */
      static PlayerExpulsion parsePex(const std::string& command);
      /**
       * @brief Parses the 'pbc' command to extract broadcast event.
       * @param command The command string.
       * @return BroadcastEvent structure.
       */
      static BroadcastEvent parsePbc(const std::string& command);
      /**
       * @brief Parses the 'smg' command to extract server message event.
       * @param command The command string.
       * @return ServerMessageEvent structure.
       */
      static ServerMessageEvent parseSmg(const std::string& command);
      /**
       * @brief Parses the 'seg' command to extract game over event.
       * @param command The command string.
       * @return GameOverEvent structure.
       */
      static GameOverEvent parseSeg(const std::string& command);

    private:
  };
}  // namespace parser
