#ifndef TILE_H
#define TILE_H

#include <gf/Vector.h>
#include <gf/Sprite.h>

namespace rCMI {  

  enum class TileType {
    Floor,
    Wall
  };

  class Tile {
    private: 

      TileType type;
      bool walkable;
      gf::Sprite sprite;

    public:

    void setTexture(const gf::Texture& texture);
    void setPosition(gf::Vector2f position);
    void render(gf::RenderTarget& renderer);
  };
};
#endif