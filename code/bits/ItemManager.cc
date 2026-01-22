#include "ItemManager.h"
#include "WorldEntity.h"
#include "GameData.h"
#include "RogueCMI.h"

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

  int ItemManager::isItemOnTile(RogueCMI *game) {
    gf::Vector2f heroGridPos = game->m_WorldScene.m_world_entity.hero().getExistence().getPosition();
    for (int i = 0; i < items.size(); i++)
    {
        gf::Vector2i itemGridPos = {
            static_cast<int>(items[i].sprite.getPosition().x / TileSize),
            static_cast<int>(items[i].sprite.getPosition().y / TileSize)
        };

        if (static_cast<int>(heroGridPos.x) == itemGridPos.x && 
            static_cast<int>(heroGridPos.y) == itemGridPos.y)
        {
          return i;
        }
    }
    return -1;
    
  }





  void ItemManager::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    // std::cout << "Nombre d'items au sol : " << items.size() << std::endl;
    for (auto &dropped : items)
      if (m_world->getMap().isInFieldOfVision(dropped.sprite.getPosition() / TileSize))
        target.draw(dropped.sprite, states);
  }
}