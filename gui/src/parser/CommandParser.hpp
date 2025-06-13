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
    std::array<int, RESOURCE_COUNT> quantities;

    TileUpdate(int x, int y, const std::array<int, RESOURCE_COUNT>& q)
      : x(x), y(y), quantities(q) {}
  };


  class CommandParser {
    public:
      CommandParser() = default;
      ~CommandParser() = default;

      static MapSize parseMsz(const std::string& command);
      static TimeUnit parseSgt(const std::string& command);
      static TeamNames parseTna(const std::string& command);
      static TileUpdate parseBct(const std::string& command);

    private:
  };
}  // namespace parser
