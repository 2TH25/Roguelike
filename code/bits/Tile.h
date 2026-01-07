// Dans Tile.h
#ifndef TILE_H
#define TILE_H

#include <gf/Vector.h>
#include <gf/Sprite.h>
#include <gf/RenderTarget.h>

namespace rCMI {
  enum class TileType { Floor, Wall };

  class Tile {
  public:

    TileType getType() const { return type; }
    bool isWalkable() const { return walkable; }
    gf::Sprite getSprite() const { return sprite; }
    gf::Vector2f getPosition() const { return pos; }

    void render(gf::RenderTarget& renderer) {
        renderer.draw(sprite);
    };

    void setPosition(gf::Vector2f position) {
        pos = position;
        sprite.setPosition(position);
    };

    void setTexture(const gf::Texture& texture) {
        sprite.setTexture(texture);
    };
    void setType(TileType t) { type = t; walkable = (t == TileType::Floor); };

  private:
    TileType type;
    bool walkable;
    gf::Sprite sprite;
    gf::Vector2f pos;
  };
};
#endif // TILE_H