#ifndef ITEM_MANAGER_H
#define ITEM_MANAGER_H

#include <vector>
#include <gf/Entity.h>
#include <gf/Sprite.h>
#include <gf/RenderTarget.h>
#include "Item.h"
#include <iostream>

namespace rCMI {

    struct DroppedItem {
        Item item;
        gf::Sprite sprite;
    };

    class RogueCMI;
    class ItemManager : public gf::Entity {
    public:
        std::vector<DroppedItem> items;

        virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override {
            std::cout << "Nombre d'items au sol : " << items.size() << std::endl;
            for (auto& dropped : items) {
                target.draw(dropped.sprite, states);
            }
        }
    };
}

#endif