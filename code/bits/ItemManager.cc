#include "ItemManager.h"
#include "WorldEntity.h"
#include "GameData.h"

namespace rCMI
{
  ItemManager::ItemManager(WorldEntity *world)
      : m_world(world)
  {
  }

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

  void ItemManager::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    // std::cout << "Nombre d'items au sol : " << items.size() << std::endl;
    for (auto &dropped : items)
      if (m_world->getMap().isInFieldOfVision(dropped.sprite.getPosition() / TileSize))
        target.draw(dropped.sprite, states);
  }
}
