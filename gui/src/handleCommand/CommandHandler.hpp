#pragma once

#include <string>
#include "../entities/Map.hpp"

namespace handlecommand {
  class CommandHandler {
    public:
      CommandHandler(gui::Map& map);
      void handleBct(const std::string& command);

    private:
      gui::Map& _map;
  };
}
