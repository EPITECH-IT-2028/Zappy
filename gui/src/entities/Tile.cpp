#include "Tile.hpp"
#include "raylib.h"

/**
 * @brief Checks if the tile is empty (no players or eggs present).
 *
 * @return true if there are no players or eggs on the tile, false otherwise.
 */
bool gui::Tile::isEmpty() const {
  return playerIdsOnTile.empty() && eggIdsOnTile.empty();
}

/**
 * @brief Gets the color associated with a given resource type.
 *
 * @param type The resource type.
 * @return The color corresponding to the specified resource.
 */
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

/**
 * @brief Gets the display name of a given resource type.
 *
 * @param type The resource type.
 * @return The name of the resource as a string.
 */
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
