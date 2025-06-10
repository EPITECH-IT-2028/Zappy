#include <string>
#include <vector>

namespace parser {
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

  class CommandParser {
    public:
      CommandParser() = default;
      ~CommandParser() = default;

      MapSize parseMsz(const std::string& command);
      TimeUnit parseSgt(const std::string& command);
      TeamNames parseTna(const std::string& command);

    private:
  };
}  // namespace parser
