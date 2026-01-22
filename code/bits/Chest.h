#ifndef CHEST_H
#define CHEST_H

#include <gf/Sprite.h>
#include "ItemManager.h"

namespace rCMI {

    struct Chest {
        gf::Sprite sprite;
        gf::Vector2i gridPos;
        bool isOpen = false;
        Item content; 
    };

}



#endif