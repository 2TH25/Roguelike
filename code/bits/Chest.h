#ifndef CHEST_H
#define CHEST_H

#include <gf/Sprite.h>
#include "ItemManager.h"

namespace rCMI {

    struct Chest {
        gf::Sprite sprite;
        gf::Vector2i gridPos;
        bool isOpen = false;
        std::vector<Item> content; 
        std::vector<int> itemSlotsPosition;
        bool distributeScore = false;
    };

}



#endif