#include "Tile.hpp"
#include "raylib.h"

bool gui::Tile::isEmpty() const {
  return playerIdsOnTile.empty() && eggIdsOnTile.empty();
}

Color gui::Tile::getResourceColor(gui::Tile::Resource type) const {
  switch (type) {
    case gui::Tile::Resource::FOOD:
      return YELLOW;
    case gui::Tile::Resource::LINEMATE:
      return GREEN;
    case gui::Tile::Resource::DERAUMERE:
      return BLUE;
    case gui::Tile::Resource::SIBUR:
      return BROWN;
    case gui::Tile::Resource::MENDIANE:
      return ORANGE;
    case gui::Tile::Resource::PHIRAS:
      return PURPLE;
    case gui::Tile::Resource::THYSTAME:
      return PINK;
    default:
      return RED;
  }
}

std::string gui::Tile::getResourceName(gui::Tile::Resource type) const {
  switch (type) {
    case gui::Tile::Resource::FOOD:
      return "Food";
    case gui::Tile::Resource::LINEMATE:
      return "Linemate";
    case gui::Tile::Resource::DERAUMERE:
      return "Deraumere";
    case gui::Tile::Resource::SIBUR:
      return "Sibur";
    case gui::Tile::Resource::MENDIANE:
      return "Mendiane";
    case gui::Tile::Resource::PHIRAS:
      return "Phiras";
    case gui::Tile::Resource::THYSTAME:
      return "Thystame";
    default:
      return "Unknown Resource";
  }
}
