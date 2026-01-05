#include "Tile.h"
#include <cassert>
#include <iterator>

namespace rl {

  namespace {
    // Utilisation de gf::Console::Cell et de gf::Color::...
    const TileData Tileset[] = {
      // Wall : Fond Noir
      { { gf::Color::White, gf::Color::Black, ' ' }, { gf::Color::White, gf::Color::Black, ' ' } }, 
      // Floor : Fond Blanc
      { { gf::Color::Black, gf::Color::White, ' ' }, { gf::Color::Black, gf::Color::White, ' ' } }, 
    };
  }

  gf::Console::Cell tile_to_light(Tile tile)
  {
    auto index = static_cast<std::size_t>(tile);
    assert(index < std::size(Tileset));
    return Tileset[index].light;
  }

  gf::Console::Cell tile_to_dark(Tile tile)
  {
    auto index = static_cast<std::size_t>(tile);
    assert(index < std::size(Tileset));
    return Tileset[index].dark;
  }
}