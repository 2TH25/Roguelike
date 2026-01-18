#ifndef INVENTORY_H
#define INVENTORY_H

#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>
#include <gf/Widget.h>
#include <gf/Event.h>
#include <gf/Rect.h>
#include <vector>
#include <iostream>
#include <gf/Sprite.h>

#include "Item.h"
#include "Stat.h"

namespace rCMI {

    class RogueCMI;

    class Inventory {
        private: 
            bool m_isOpen = false;
            gf::WidgetContainer m_container;
            Stat m_stats;
            gf::TextWidget m_statsWidget;

            gf::Sprite m_heroSprite;

            gf::SpriteWidget m_headSlot;
            gf::SpriteWidget m_legSlot;
            gf::SpriteWidget m_torsoSlot;
            gf::SpriteWidget m_handSlot;
            gf::SpriteWidget m_feetSlot;
            gf::SpriteWidget m_accessorySlot;
            std::vector<Item> m_backpack;

            Item* m_equippedHead = nullptr;
            Item* m_equippedTorso = nullptr;
            Item* m_equippedLegs = nullptr;
            Item* m_equippedHand = nullptr;
            Item* m_equippedFeet = nullptr;
            Item* m_equippedAccessory = nullptr;

            gf::RectangleShape m_background;

            gf::SpriteWidget* getWidgetByType(ItemType type);

        public: 
            Inventory(RogueCMI *game);

            bool hasEquipment(ItemType type);
            void setEquippedItem(ItemType type, Item* item, RogueCMI *game);
            Item* getEquippedItem(ItemType type);
            void onUnequip(ItemType type, RogueCMI *game);
            void updateStatsText();
            void render(gf::RenderTarget& target, const gf::RenderStates& states);
            void updateInventory(RogueCMI *game);
    };
}

#endif