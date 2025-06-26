#include "Map.hpp"
#include <stdexcept>
#include <string>

void gui::Map::resize(std::size_t w, std::size_t h) {
  if (w == 0 || h == 0)
    throw std::invalid_argument("Map dimensions must be greater than zero");
  if (w == width && h == height)
    return;

  width = w;
  height = h;
  tiles.resize(height);
  for (auto& row : tiles)
    row.resize(width);
}

gui::Tile& gui::Map::getTile(std::size_t x, std::size_t y) {
  ensureInBounds(x, y);
  return tiles[y][x];
}

const gui::Tile& gui::Map::getTile(std::size_t x, std::size_t y) const {
  ensureInBounds(x, y);
  return tiles[y][x];
}

bool gui::Map::isInside(std::size_t x, std::size_t y) const {
  return x < width && y < height;
}

void gui::Map::ensureInBounds(std::size_t x, std::size_t y) const {
  if (x >= width || y >= height) {
    throw std::out_of_range("Tile coordinates out of map (" +
                            std::to_string(x) + ", " + std::to_string(y) + ")");
  }
}
