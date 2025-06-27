#include "Tile.hpp"
#include <iostream>
#include "raylib.h"

bool gui::Tile::isEmpty() const {
  return playerIdsOnTile.empty() && eggIdsOnTile.empty();
}
