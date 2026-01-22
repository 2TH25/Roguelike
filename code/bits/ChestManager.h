#ifndef CHESTMANAGER_H
#define CHESTMANAGER_H

#include "Chest.h"

namespace rCMI {

    class RogueCMI;
    class ChestManager {
        public:
            std::vector<Chest> m_chests;
            void update(RogueCMI *game);
            void openChest(Chest& chest,RogueCMI *game);
            void spawnChest(gf::Vector2i pos, RogueCMI *game);
            void render(gf::RenderTarget &target, const gf::RenderStates &states);
    };
}

#endif