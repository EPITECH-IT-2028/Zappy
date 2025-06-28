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
