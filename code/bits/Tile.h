#ifndef TILE_H
#define TILE_H

#include <gf/Sprite.h>
#include <gf/RenderTarget.h>

namespace rCMI {
  enum class TileType { Floor, Wall };

  class Tile {
  public:
    void setTexture(const gf::Texture& tex) { m_sprite.setTexture(tex); }
    void setPosition(gf::Vector2f pos) { m_sprite.setPosition(pos); }
    void render(gf::RenderTarget& target) { target.draw(m_sprite); }

  private:
    gf::Sprite m_sprite;
    TileType m_type;
  };
}
#endif // TILE_H