#include "Tile.h"

namespace rCMI {
    class Tile {
        
        void render(gf::RenderTarget& renderer) {
            renderer.draw(sprite);
        }
    }
};
