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

namespace rCMI
{

  class RogueCMI;

  class Inventory
  {
  private:
    bool m_isOpen = false;
    
    Stat m_stats;
    gf::TextWidget m_statsWidget;

    gf::Sprite m_heroSprite;

    gf::SpriteWidget m_headSlot;
    gf::SpriteWidget m_legSlot;
    gf::SpriteWidget m_torsoSlot;
    gf::SpriteWidget m_handSlot;
    gf::SpriteWidget m_bootsSlot;
    gf::SpriteWidget m_accessorySlot;

    std::vector<Item> m_backpack;
    static constexpr std::size_t MaxBackpackSize = 10;
    std::array<gf::SpriteWidget, MaxBackpackSize> m_backpackWidgets;
    gf::Texture *m_emptySlotTexture; // slot vide du backpack

    struct EquippedSlot
    {
      Item item;
      bool hasItem = false;
    };

    EquippedSlot m_equippedHead;
    EquippedSlot m_equippedTorso;
    EquippedSlot m_equippedLegs;
    EquippedSlot m_equippedHand;
    EquippedSlot m_equippedBoots;
    EquippedSlot m_equippedAccessory;

    gf::RectangleShape m_background;

    gf::SpriteWidget *getWidgetByType(ItemType type);

  public:
    Inventory(RogueCMI *game);

    gf::WidgetContainer m_container;

    bool hasEquipment(ItemType type);
    void setEquippedItem(ItemType type, Item *item, RogueCMI *game);
    Item getEquippedItem(ItemType type);
    void onUnequip(ItemType type, RogueCMI *game);
    void updateStatsText();
    void render(gf::RenderTarget &target, const gf::RenderStates &states);
    void updateInventory(RogueCMI *game);
    void updateBackpackDisplay(RogueCMI *game);
    void equipFromBackpack(std::size_t index, RogueCMI *game);
    EquippedSlot *getSlotByType(ItemType type);
    bool addItemToBackpack(Item item, RogueCMI *game);
    void handleItemClick(gf::Vector2f coords, RogueCMI *game);
  };
}

#endif