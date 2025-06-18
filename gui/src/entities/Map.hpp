#pragma once
#include <cstddef>
#include <stdexcept>
#include <vector>
#include "Tile.hpp"

namespace gui {
  struct Map {  
    std::size_t width;
    std::size_t height;
    std::vector<std::vector<Tile>> tiles;

    Map(std::size_t w, std::size_t h) : width(w), height(h), tiles(h, std::vector<Tile>(w)) {}

  private:
    void ensureInBounds(std::size_t x, std::size_t y) const {
      if (x >= width || y >= height)
        throw std::out_of_range("Tile coordinates out of map (" +
          std::to_string(x) + ", " + std::to_string(y) + ")");
    }

  public:
    Tile& getTile(std::size_t x, std::size_t y) {
      ensureInBounds(x, y);
      return tiles[y][x];
    }

    const Tile& getTile(std::size_t x, std::size_t y) const {
       ensureInBounds(x, y);
       return tiles[y][x];
     }
    bool isInside(std::size_t x, std::size_t y) const {
      return x < width && y < height;
    }
  };
}