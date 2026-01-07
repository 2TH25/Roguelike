#include "Tile.h"

namespace rCMI {
    class Tile {

        void Tile::render(gf::RenderTarget& renderer) {
            renderer.draw(sprite);
        }

        void Tile::setPosition(gf::Vector2f position) {
            pos = position;
            sprite.setPosition(position);
        }

        void setTexture(const gf::Texture& texture) {
            sprite.setTexture(texture);
        }
    }
};
