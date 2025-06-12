#pragma once
#include <stdexcept>
#include <vector>
#include "Tile.hpp"

namespace gui {
  struct Map {  
    int width;
    int height;
    std::vector<std::vector<Tile>> tiles;

    Map(int w, int h) : width(w), height(h), tiles(h, std::vector<Tile>(w)) {}

  private:
    void ensureInBounds(int x, int y) const {
      if (x >= width || y >= height)
        throw std::out_of_range("Tile coordinates out of map");
    }

  public:
    Tile& getTile(int x, int y) {
      ensureInBounds(x, y);
      return tiles[y][x];
    }

    const Tile& getTile(int x, int y) const {
       ensureInBounds(x, y);
       return tiles[y][x];
     }
  };
}