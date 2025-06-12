#pragma once
#include <stdexcept>
#include <vector>
#include "Tile.hpp"

namespace gui {
  struct Map {  
    std::size_t width;
    std::size_t height;
    std::vector<std::vector<Tile>> tiles;

    Map(std::size_t w, std::size_t h) : width(w), height(h), tiles(h, std::vector<Tile>(w)) {}

  Tile& getTile(std::size_t x, std::size_t y) {
    if (x >= width || y >= height)
      throw std::out_of_range("Tile coordinates out of map");
    return tiles[y][x];
    }

  const Tile& getTile(std::size_t x, std::size_t y) const {
    return const_cast<Map&>(*this).getTile(x, y);
    }
  };
}