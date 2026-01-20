#ifndef ITEM_MANAGER_H
#define ITEM_MANAGER_H

#include <vector>
#include <gf/Entity.h>
#include <gf/Sprite.h>
#include <gf/RenderTarget.h>
#include "Item.h"
#include <iostream>

namespace rCMI
{

  struct DroppedItem
  {
    Item item;
    gf::Sprite sprite;
  };

  class RogueCMI;
  class WorldEntity;

  class ItemManager
  {
  public:
    std::vector<DroppedItem> items;
    WorldEntity *m_world;
    ItemManager(WorldEntity *world);

    virtual void render(gf::RenderTarget &target, const gf::RenderStates &states);
    void spawnItem(gf::Vector2f position, RogueCMI *game);
  };
}

#endif