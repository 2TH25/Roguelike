#include "ItemManager.h"

namespace rCMI {

    void ItemManager::spawnItem(gf::Vector2f position, RogueCMI *game)
  {
    Item newItem = Item::generateRandomItem(game);

    DroppedItem dropped;
    dropped.item = newItem;
    if (newItem.m_texture)
      dropped.sprite.setTexture(*newItem.m_texture);

    gf::Vector2f size = dropped.sprite.getLocalBounds().getSize();

    dropped.sprite.setOrigin({size.x / 2.0f, size.y / 2.0f});

    dropped.sprite.setPosition(position);
    dropped.sprite.setScale({0.5f, 0.5f});

    items.push_back(dropped);
  }

}

