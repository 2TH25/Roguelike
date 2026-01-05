#ifndef TILE_H
#define TILE_H

#include <gf/Console.h>
#include <gf/Color.h>
#include <cstdint>

namespace rl {

  enum class Tile : uint16_t {
    Wall = 0,
    Floor = 1,
  };

  struct TileData {
    gf::Console::Cell light;
    gf::Console::Cell dark;  
  };

  gf::Console::Cell tile_to_light(Tile tile);
  gf::Console::Cell tile_to_dark(Tile tile);
}

#endif