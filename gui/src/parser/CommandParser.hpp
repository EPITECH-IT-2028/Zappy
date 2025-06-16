#pragma once

#include <string>
#include <vector>
#include <array>

namespace parser {
  inline constexpr std::size_t RESOURCE_COUNT = 7;

  struct MapSize {
      int width;
      int height;
      MapSize(int w, int h) : width(w), height(h) {
      }
  };
  struct TimeUnit {
      int time;
      TimeUnit(int t) : time(t) {
      }
  };
  struct TeamNames {
      std::vector<std::string> names;
      TeamNames(const std::vector<std::string>& n) : names(n) {
      }
  };
  struct TileUpdate {
    int x;
    int y;
    std::array<int, RESOURCE_COUNT> resources;

    TileUpdate(int x, int y, const std::array<int, RESOURCE_COUNT>& q)
      : x(x), y(y), resources(q) {}
  };
  struct PlayerInfo {
    int id;
    int x;
    int y;
    int orientation;
    int level;
    std::string teamName;

    PlayerInfo(int id, int x, int y, int orientation, int level, const std::string& team)
        : id(id), x(x), y(y), orientation(orientation), level(level), teamName(team) {}
  };
  struct PlayerPositionUpdate {
    int id;
    int x;
    int y;
    int orientation;

    PlayerPositionUpdate(int id, int x, int y, int orientation)
        : id(id), x(x), y(y), orientation(orientation) {}
  };
  struct PlayerLevelUpdate {
    int id;
    int level;

    PlayerLevelUpdate(int id, int level) : id(id), level(level) {}
  };
  struct PlayerInventory {
    int id;
    int x;
    int y;
    std::array<int, RESOURCE_COUNT> resources;

    PlayerInventory(int id, int x, int y, const std::array<int, RESOURCE_COUNT>& res)
        : id(id), x(x), y(y), resources(res) {}
  };
  struct EggLaid {
    int idEgg;
    int idPlayer;
    int x;
    int y;

    EggLaid(int idEgg, int idPlayer, int x, int y) : idEgg(idEgg), idPlayer(idPlayer), x(x), y(y) {}
  };
  struct EggHatch {
    int eggId;
    EggHatch(int id) : eggId(id) {}
  };
  struct EggDeath {
    int eggId;
    EggDeath(int id) : eggId(id) {}
  };
  struct PlayerDeath {
    int id;
    PlayerDeath(int id) : id(id) {}
  };
  
  class CommandParser {
    public:
      CommandParser() = default;
      ~CommandParser() = default;

      static MapSize parseMsz(const std::string& command);
      static TimeUnit parseSgt(const std::string& command);
      static TeamNames parseTna(const std::string& command);
      static TileUpdate parseBct(const std::string& command);
      static PlayerInfo parsePnw(const std::string& command);
      static PlayerPositionUpdate parsePpo(const std::string& command);
      static PlayerLevelUpdate parsePlv(const std::string& command);
      static PlayerInventory parsePin(const std::string& command);
      static EggLaid parseEnw(const std::string& command);
      static EggHatch parseEbo(const std::string& command);
      static EggDeath parseEdi(const std::string& command);
      static PlayerDeath parsePdi(const std::string& command);

    private:
  };
}  // namespace parser
